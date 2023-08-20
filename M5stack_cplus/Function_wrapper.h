#include "QDP_text_code.h"
#include <driver/i2s.h>

#define CONFIG_I2S_BCK_PIN -1
#define CONFIG_I2S_LRCK_PIN 0
#define CONFIG_I2S_DATA_PIN -1
#define CONFIG_I2S_DATA_IN_PIN 34

#define SPEAK_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1

char* utf8togb2312(String input_data)
{
  int input_num = input_data.length();
  int output_num = 0;
  unsigned char str[input_num];
  byte select = 0;
  for (int x = 0; x < input_num; x++)
  {
    str[x] = input_data.charAt(x);
    select = Transform.GetUtf8ByteNumForWord(str[x]);
    if (select == 0)
      output_num++;
    else if (select >= 2)
      output_num += 2;
  }
  uint8_t gbArray[output_num];
  Transform.Utf8ToGb2312(str, input_num, gbArray);
  char gbArray1[output_num];
  static String data;
  data = "";
  for (int x = 0; x < output_num; x++)
  {
    data = String(data) + String(char(gbArray[x]));
  }
  char* gbkstr = const_cast<char*>(data.c_str());
  return gbkstr;
}

bool InitI2SSpeakOrMic(int mode)
{
  esp_err_t err = ESP_OK;

  i2s_driver_uninstall(SPEAK_I2S_NUMBER);
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
     #if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
     #else
         .communication_format = I2S_COMM_FORMAT_I2S,
     #endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 6,
    .dma_buf_len = 60,
  };
  if (mode == MODE_MIC)
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  }
  else
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    i2s_config.use_apll = false;
    i2s_config.tx_desc_auto_clear = true;
  }

  Serial.println("Init i2s_driver_install");

  err += i2s_driver_install(SPEAK_I2S_NUMBER, &i2s_config, 0, NULL);
  i2s_pin_config_t tx_pin_config;
  #if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    tx_pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
  #endif
  tx_pin_config.bck_io_num = -1;//I2S_PIN_NO_CHANGE;
  tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
  tx_pin_config.data_out_num = -1;//I2S_PIN_NO_CHANGE;
  tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;

  Serial.println("Init i2s_set_pin");
  err += i2s_set_pin(SPEAK_I2S_NUMBER, &tx_pin_config);
  //err +=    i2s_set_pin(I2S_NUM_0, &pin_config);
  Serial.println("Init i2s_set_clk");
  err += i2s_set_clk(SPEAK_I2S_NUMBER, 16000, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
  //err += i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO); 
  return true;
}
