#include "lcd.h"
#include "spi.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "display.h"
#include "button.h"

DRAM_ATTR static const lcd_init_cmd_t ili_init_cmds[] = {
    /* Power control B, power control = 0, DC_ENA = 1 */
    {0xCF, {0x00, 0x83, 0x30}, 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 1, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {0xED, {0x64, 0x03, 0x12, 0x81}, 4},
    /* Driver timing control A,
     * non-verlap=default+1
     * EQ=default-1, CR=default
     * pre-charge=default-1
     */
    {0xE8, {0x85, 0x01, 0x79}, 3},
    /* Power control A, vCore=1.6V, DDVDH=5.6V */
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    /* Pump ratio contro, DDVDH=2xVCL */
    {0xF7, {0x20}, 1},
    /* Driver timing control, all=0, unit */
    {0xEA, {0x00, 0x00}, 2},
    /* Power control 1, GVDD=4.75V */
    {0xC0, {0x26}, 1},
    /* Power control 2, DDVDH=VCL*2, VGH=VCL*7, VGL=-VCL*3 */
    {0xC1, {0x11}, 1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {0xC5, {0x35, 0x3E}, 2},
    /* CVOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {0xC7, {0xBE}, 1},
    /* Memory access control, MX=MY=0, MV=1, ML=0, BGR=1, MH=0 */
    {0x36, {0x28}, 1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},
    /* Frame rate control, f=fosc, 70Hz fps */
    {0xB1, {0x00, 0x1B}, 2},
    /* Enable 3G, disabled */
    {0xF2, {0x08}, 1},
    /* Gamma set, curve 1 */
    {0x26, {0x01}, 1},
    /* Positive gamma correction */
    {0xE0, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0x87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
    /* Negative gamma correction */
    {0xE1, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
    /* Column address set, SC=0, EC=0xEF */
    {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    /* Page address setm SP=0, EP=0x013F */
    {0x2B, {0x00, 0x00, 0x01, 0x3F}, 4},
    /* Memory write */
    {0x2C, {0}, 0},
    /* Entry mode set, Low vol detected disabled, normal display */
    {0xB7, {0x07}, 1},
    /* Display function control */
    {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
    /* Sleep out */
    {0x11, {0}, 0x80},
    /* Display on */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff},
};

uint16_t *lines[2];
int frame;
int sending_line;
int calc_line;

/* Send a command to the LCD. Uses spi_device_polling_transmit, which wiats
 * until the transfer is complete.
 *
 * Since command transactions are sually small, they are handles in polling
 * mode for higher speed. The overhead of interrup transactions is more than
 * just waiting for thge transaction to complete.
 */
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       // Zero out the transaction
    t.length = 8;                   // Command is 8 bits
    t.tx_buffer = &cmd;             // The data is the cmd itself
    t.user = (void*)0;              // D/C needs to be set to 0
    if (keep_cs_active) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE;     // Keep CS active after data transfer
    }
    ret = spi_device_polling_transmit(spi, &t); // Transmit
    assert(ret == ESP_OK);          // Should have no issues
}


/* Send data to the LCD. Uses spi_devie_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handles in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0) {
        return;     // No need to send anything
    }
    memset(&t, 0, sizeof(t));       // Zero out the transaction
    t.length = len * 8;             // Len is in bytes, transaction length is in bits
    t.tx_buffer = data;             // Data
    t.user = (void*)1;               // D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t); // Transmit
    assert(ret == ESP_OK);          // Should have no issues
}

// This function is called (in irq context) just before a transmission starts. it will
// set the D/C line to the value indicated in the user field.
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(LCD_PIN_NUM_DC, dc);
}

uint32_t lcd_get_id(spi_device_handle_t spi)
{
    // When using SPI_TRANS_CS_KEEP_ACTIVE, bus must be locked/acquired
    spi_device_acquire_bus(spi, portMAX_DELAY);
    
    // get_id cmd
    lcd_cmd(spi, 0x04, true);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8 * 3;
    t.flags = SPI_TRANS_USE_RXDATA;
    t.user = (void*)1;

    esp_err_t ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);

    // Release bus
    spi_device_release_bus(spi);

    return *(uint32_t*)t.rx_data;
}

// Initialize the display
void lcd_init(spi_device_handle_t spi)
{
    int cmd = 0;

    // Initialize non-PI GPIOs
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = ((1ULL << LCD_PIN_NUM_DC) | (1ULL << LCD_PIN_NUM_BCKL) | (1ULL << LCD_PIN_NUM_RST));
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = true;
    esp_err_t result = gpio_config(&io_conf);
    
    /*if(result != ESP_OK)
        printf("GPIO for LCD could not be setup\n");*/

    // Reset the display
    gpio_set_level(LCD_PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(LCD_PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Send all the commands
    while (ili_init_cmds[cmd].databytes != 0xFF) {
        lcd_cmd(spi, ili_init_cmds[cmd].cmd, false);
        lcd_data(spi, ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes & 0x1F);
        if(ili_init_cmds[cmd].databytes & 0x80) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        cmd++;
    }

    // Enable backlight
    gpio_set_level(LCD_PIN_NUM_BCKL, LCD_BK_LIGHT_ON_LEVEL);
}


/* To send a set of lines we have to send a comand, 2 data bytes, another command, 2 more data bytes and another command
 * before sending the line data itself; a total of 6 transactions. (We can't put all of this is just one transaction
 * because the D/C line needs to be toggled in the middle.)
 * This routine queues these commands up as interrupt transactions so they gey
 * sent faster (compared to calling spi_device_transmit several times), and at
 * the mean while the lines for next transactions can get calculated.
 */
void send_lines(spi_device_handle_t spi, int ypos, uint16_t *linedata)
{
    esp_err_t ret;
    int x;
    // Transaction descriptors. Declared static so they're not allocated on the stack; we need this memory even when this
    // function is finished because the SPI driver needs access to it even while we're already calculating the next line.
    static spi_transaction_t trans[6];

    // In theory, it's better to initialize trans and data only once and hang on to the initializd
    // variables. We allocated them on the stack, so we need to re-init them each call.
    for (x = 0; x < 6; x++) {
        memset(&trans[x], 0, sizeof(spi_transaction_t));
        if((x & 1) == 0) {
            // Even transfers are commands
            trans[x].length = 8;
            trans[x].user = (void*)0;
        } else {
            // Odd transfers are data
            trans[x].length = 8 * 4;
            trans[x].user = (void*)1;
        }
        trans[x].flags = SPI_TRANS_USE_TXDATA;
    }
    trans[0].tx_data[0] = 0x2A;         // Column Address Set
    trans[1].tx_data[0] = 0;            // Start col high
    trans[1].tx_data[1] = 0;            // Start col low
    trans[1].tx_data[2] = (320) >> 8;   // End col hight
    trans[1].tx_data[3] = (320) & 0xFF; // End col low
    trans[2].tx_data[0] = 0x2B;         // Page Address Set
    trans[3].tx_data[0] = ypos >> 8;    // Start page high
    trans[3].tx_data[1] = ypos & 0xFF;  // Start page low
    trans[3].tx_data[2] = (ypos + PARALLEL_LINES) >> 8; // End page high
    trans[3].tx_data[3] = (ypos + PARALLEL_LINES) & 0xFF; // End page low
    trans[4].tx_data[0] = 0x2C;         // Memory write
    trans[5].tx_buffer = linedata;      // Finally send the line data
    trans[5].length = 320 * 2 * 8 * PARALLEL_LINES; // Data length, in bits
    trans[5].flags = 0;                 // Undo SPI_TRANS_USE_TXDATA_FLAG
    
    // Queue all transactions.
    for (x = 0; x < 6; x++) {
        ret = spi_device_queue_trans(spi, &trans[x], portMAX_DELAY);
        assert(ret == ESP_OK);
    }


    // When we are here, the SPI driver is busy (in the background) getting the transactions sent. That happens
    // mostly using DMA, so the CPU doesn't have much to do here. We're not going to wait f or the transactions to
    // finish because we may as well spend the time calculating the next line. When that is done, we can call
    // send_line_finish, which will wait for the transfers to be done and check their status.
}


void send_line_finish(spi_device_handle_t spi)
{
    spi_transaction_t *rtrans;
    esp_err_t ret;
    // Wait for all 6 transactions to be done and get back the results
    for (int x = 0; x < 6; x++) {
        ret = spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY);
        //assert(ret = ESP_OK);
        // We could inspect rtrans now if we received any info back. The LCD is treated as write-onlu, though.
    }
}


void display_lcd_prepare()
{
    //uint16_t *lines[2];
    // Allocate memory for the pixel buffers
    for (int i = 0; i < 2; i++) {
        lines[i] = heap_caps_malloc(320 * PARALLEL_LINES * sizeof(uint16_t), MALLOC_CAP_DMA);
        assert(lines[i] != NULL);
    }
    frame = 0;
    // Indexes of the line currently being sent to the LCD and the line we're calculating.
    sending_line = -1;
    calc_line = 0;

}

void display_lcd_screen(spi_device_handle_t spi)
{
    frame++;
    for(int y = 0; y < 240; y += PARALLEL_LINES) {
        // Calculate a line
        get_screen_lines(lines[calc_line], y, PARALLEL_LINES);

        // Finish up the sending process of the previous line, if any
        if (sending_line != -1) {
            send_line_finish(spi);
        }

        // Swap sendinng_line and calc_line
        sending_line = calc_line;
        calc_line = (calc_line == 1) ? 0 : 1;

        // Send the line we currently calculated
        send_lines(spi, y, lines[sending_line]);
        // The line set is queued up for sending now; the actual sending happens in the
        // background. We can go on to calculate the next line set as long as we do not
        // touch line[sending_line]; the SPI sending proces is still reading from that.
    }
}
