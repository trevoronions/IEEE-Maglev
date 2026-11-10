
// INITIALIZE I2C
// INITIALIZE electromagnet board addresses
const uint8_t BOARD_ADDRESSES[4] = {0x20, 0x21, 0x22, 0x23};

#define NUM_BOARDS 4
#define MAGNETS_PER_BOARD 4
#define MAGNET_CONTROL 0x01

// Command Variables
volatile uint8_t target_board = 0;
volatile uint8_t target_magnet = 0; 
volatile uint8_t magnet_state = 0;
volatile uint8_t new_command = 0;

HAL_StatusTypeDef Set_Magnet(uint8_t board, uint8_t magnet, uint8_t state){
    if (board >= NUM_BOARDS ||
        magnet >= MAGNETS_PER_BOARD ||
        state > 1) 
        {
            return HAL_ERROR;
        }
    uint8_t command[3] = {
        MAGNET_CONTROL, magnet, state
    };
    //HAL expects 7-bit addr to be shifted left
    uint16_t address = BOARD_ADDRESSES[board] << 1;

    return HAL_I2C_Master_Transmit(
        &hi2c1, address, command, sizeof(command), 100);
}



int main(void){
    
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    
    while (1){
        if (new_command == 1){
            uint8_t board = target_board;
            uint8_t magnet = target_magnet;
            uint8_t state = magnet_state;

            // Clear command flag
            new_command = 0;

            // Transmit command to selected board
            HAL_StatusTypeDef status = Set_Magnet(board, magnet, state);

            if (status != HAL_OK)
            {
                // Handle I2C communication error
            }
        }
    }
}
