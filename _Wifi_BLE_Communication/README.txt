Modify main.c file with your Access Point details before building and loading the application:

 line 65 | #define SSID_NAME       "<Your_AP's_SSID>"       /* Access point name to connect to. */
 line 66 | #define SEC_TYPE        SL_SEC_TYPE_WPA_WPA2     /* Security type of the Access piont */
 line 67 | #define PASSKEY         "<Your_AP's_Password>"   /* Password in case of secure AP */
 line 68 | #define PASSKEY_LEN     pal_Strlen(PASSKEY)      /* Password length in case of secure AP */

After the CC3100 is connected to the internet, any public Twitter message in the following format
will change the RGB LED color on all MSP-EXP432P401R LaunchPad running this demo:

  RGB(red_value, green_value, blue_value) #MSP432LaunchPad

The color parameters can be integers ranging from 0-255.

Pressing the left push button S1 on the LaunchPad publishes a 32-bit unique ID from the LaunchPad
to the MQTT broker, which then gets tweeted by the twitter account: @MSPLaunchPad

You may then use this 32-bit Unique ID in your tweet message to control the RGB LED of only the specific
LaunchPad+CC3100BOOST combination tied to that unique ID:

  <32-bit unique ID> RGB(red_value, green_value, blue_value) #MSP432LaunchPad