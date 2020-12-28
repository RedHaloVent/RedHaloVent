/*
* REDHALO LLC. All Rights Reserved.
* This version is the first actual alpha.
* 
* Connection with Arduino Uno/Nano:
* +5V = 5V
* TX  = none
* RX  = pin 1 (TX)
* GND = GND
* 
* Section 0: Repo Inclusion and Definitions
* Section 1: Initializes and makes everything work
* Section 2: Runs all of the conditionals and actually does the code
* Section 3: Finalizes all of the Aesthetics
* Section 4: Troubleshoots
* Section 5: Killswitch
 */

//SECTION 0-1: Repositories
#include "Nextion.h"

//Definitions
//Graph Define
#define LEVEL_HIGH (120)
#define LEVEL_LOW  (0)
#define CH0_OFFSET (160 - LEVEL_HIGH/2)
#define CH1_OFFSET (CH0_OFFSET + 160 * 1)
#define CH2_OFFSET (CH0_OFFSET + 160 * 2)
#define CH3_OFFSET (CH0_OFFSET + 160 * 3)
#define CH4_OFFSET (160 - LEVEL_HIGH/2)
#define CH5_OFFSET (CH4_OFFSET + 160 * 1)
#define CH6_OFFSET (CH4_OFFSET + 160 * 2)
#define CH7_OFFSET (CH4_OFFSET + 160 * 3)

//Section 1: Initializes and makes everything work
//SECTION 1-1 Initialize the variables
//Pages
NexPage page0 = NexPage(0,0,"PWRON"); //Power On: Initializes the whole thing Tree 0.
NexPage page1 = NexPage(1,0,"MMENU"); //Main Menu: Sends you to the Graph, Presets, or Settings. Tree 1
NexPage page2 = NexPage(2,0,"GRAPH"); //Graphs: The graphs that actually matter. Tree 1
NexPage page3 = NexPage(3,0,"PSETS"); //Presets: The presets for the vent. Tree 1
NexPage page4 = NexPage(4,0,"SETTS"); //Settings: Gives info on the vent, what the siren tones mean, etc. Tree 1
NexPage page5 = NexPage(5,0,"GSETT"); //Graph Settings: Allows variables to be switched on the graph in GRAPH. Tree 2. (Subset of GRAPH)
NexPage page6 = NexPage(6,0,"SCSET"); //Screen Settings: Subset of SETTS. Tree 2
NexPage page7 = NexPage(7,0,"MNSET"); //Manual Settings: Explains what all of the alerts mean and what we do. All text and legal stuff basically. Tree 2, subset of SETTS
NexPage page8 = NexPage(8,0,"DGSET"); //Diagnostics: Tree 2. Subset of GRAPHS

//Buttons
//(Page, Component #, Component ID)
NexButton b00 = NexButton(0,1,"PWRB0");  //Main Power Button: STARTUP BUTTON

NexButton b01 = NexButton(1,2,"PWRMM");  //Power Button Menu Page: SHUTDOWN BUTTON
NexButton b02 = NexButton(2,3,"PWRGH");  //Power Button Graph Page:
NexButton b03 = NexButton(3,4,"PWRPS");  //Power Button Preset Page:
NexButton b04 = NexButton(4,5,"PWRST");  //Power Button Settings Page:
NexButton b05 = NexButton(5,6,"PWRGS");  //Power Button Graph Settings Page:
NexButton b06 = NexButton(6,7,"PWRSC");  //Power Button Screen Settings Page:
NexButton b07 = NexButton(7,8,"PWRMN");  //Power Button Manual Settings Page:
NexButton b08 = NexButton(8,9,"PWRDG");  //Power Button Diagnostics Page:

NexButton b09 = NexButton(1,10,"HMEMM"); //Home Button Menu Page: SENDS USER TO MAIN MENU
NexButton b10 = NexButton(2,11,"HMEGH"); //Home Button Graph Page:
NexButton b11 = NexButton(3,12,"HMEPS"); //Home Button Preset Page:
NexButton b12 = NexButton(4,13,"HMEST"); //Home Button Settings Page:
NexButton b13 = NexButton(5,14,"HMEGS"); //Home Button Graph Settings Page:
NexButton b14 = NexButton(6,15,"HMESC"); //Home Button Screen Settings Page:
NexButton b15 = NexButton(7,16,"HMEMN"); //Home Button Manual Settings Page:
NexButton b16 = NexButton(8,17,"HMEDG"); //Home Button Diagnostics Page:

NexButton b17 = NexButton(1,18,"S2GPG"); //Send To Graph Page: SENDS USER TO A SPECIFIC PAGE
NexButton b18 = NexButton(1,19,"S2PPG"); //Send To Preset Page:
NexButton b19 = NexButton(1,20,"S2SPG"); //Send To Settings Page:

NexButton b20 = NexButton(2,21,"S2GSP"); //Send To Graph Settings Page:
NexButton b21 = NexButton(3,22,"S2SCP"); //Send To Screen Settings Page:
NexButton b22 = NexButton(4,23,"S2MNP"); //Send To Manual Settings Page:
NexButton b23 = NexButton(2,24,"S2DGP"); //Send To Diagnostics Page:
NexButton b24 = NexButton(5,25,"S2GGS"); //Send to Graph From Graph Settings:
NexButton b25 = NexButton(8,26,"S2GDG"); //Send to Graph From Diagnostics:

//Dual State Buttons
NexDSButton bt0 = NexDSButton(6,27,"Tenative"); //I have no idea what I'll do with this

//Graphs (I have to recreate the same graph for each page.
//(Page, Component #, Component ID)
NexWaveform s0 = NexWaveform(2,28,"TVOL"); //Tidal Volume Graph Page: RESX- 10ms, RESY- 50mL
NexWaveform s1 = NexWaveform(5,29,"TVGS"); //Tidal Volume Graph Settings: RangeY - 0-500mL
NexWaveform s2 = NexWaveform(8,30,"TVDG"); //Tidal Volume Diagnostics: RangeX - 0-10s

NexWaveform s3 = NexWaveform(2,31,"VFLR"); //Volume Flow Graph: RESX- 10ms, RESY- 50mL
NexWaveform s4 = NexWaveform(5,32,"VFGS"); //Volume Flow Graph Settings: RangeY - 60-120L
NexWaveform s5 = NexWaveform(8,33,"VFDG"); //Volume Flow Diagnostics: RangeX - 0-10s

//Pictures
NexPicture p0 = NexPicture(0,34,"TITL");
NexPicture p1 = NexPicture(0,34,"TTL2");

//SECTION 1-2 Initialize the Components
//Page lister
NexTouch * nex_listen_list[] =
{
  &page0,
  &page1,
  &page2,
  &page3,
  &page4,
  &page5,
  &page6,
  &page7,
  &page8,
  NULL
};

//Button Setter
NexTouch *nex_listen_list[] = 
{
    &b00,
    &b01,
    &b02,
    &b03,
    &b04,
    &b05,
    &b06,
    &b07,
    &b08,
    &b09,
    &b10,
    &b11,
    &b12,
    &b13,
    &b14,
    &b15,
    &b16,
    &b17,
    &b18,
    &b19,
    &b20,
    &b21,
    &b22,
    &b23,
    &b24,
    &b25,
    NULL
};

NexTouch *nex_listen_list[] = 
{
    &p0,
    &p1,
    NULL
};

//SECTION 1-3 POPCALLBACKS
void b00PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b01PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b02PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b03PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b04PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b05PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b06PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b07PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b08PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b09PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b10PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b11PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b12PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b13PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b14PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b15PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b16PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b17PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b18PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b19PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b20PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b21PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b22PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b23PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b24PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

void b25PopCallback(void *ptr)
{
    //Length of the array
    uint16_t len;
    //Allows floats within the array.
    uint16_t number;
    NexButton *btn = (NexButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    //Fills a block of memory with the buffer, a 0, and the size of the buffer.
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    btn->getText(buffer, sizeof(buffer));

    //Converts a string to an integer in the buffer and adds one 1 to the value.
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    //Converts the integer to a string
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    btn->setText(buffer);
}

//THIS CONTAINS THE COMMANDS THAT ARE NEEDED FOR THIS PARTICULAR BUTTON. ANY KIND OF PRESS OR RELEASE EVENTS
void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
    nexLoop(nex_listen_list);
}

//DS Button Setter

//Graph Setter
static uint16_t ch0_data = LEVEL_LOW; //This set is for the Tidal Volume Data
static uint16_t ch1_data = LEVEL_LOW;
static uint16_t ch2_data = LEVEL_LOW;
static uint16_t ch3_data = LEVEL_LOW;

static uint16_t ch4_data = LEVEL_LOW; //This set is for the Volume Flow Data
static uint16_t ch5_data = LEVEL_LOW;
static uint16_t ch6_data = LEVEL_LOW;
static uint16_t ch7_data = LEVEL_LOW;

//Picture Setter
void p0PopCallback(void *ptr)
{
    uint32_t number = 0;
    dbSerialPrintln("p0PopCallback");

    //Calls for the pic that we need
    p0.getPic(&number);

    if (number == 1)
    {
        number = 2;    
    }
    else
    {
        number = 1;
    }
    
    p0.setPic(number);
}

void p1PopCallback(void *ptr)
{
    uint32_t number = 0;
    dbSerialPrintln("p0PopCallback");

    //Calls for the pic that we need
    p1.getPic(&number);

    if (number == 1)
    {
        number = 2;    
    }
    else
    {
        number = 1;
    }
    
    p1.setPic(number);
}

//SECTION 1-2 Initialize the screen
//Initializing the screen iself
void setup(void)
{
  nexInit();
  dbSerialPrintln("Setup Complete");
}

void loop(void)
{
  static uint64_t started = 0;
  if (millis() - started >= 10)
  {
       started = millis();    
        if (LEVEL_HIGH == ch0_data)
        {
            ch0_data = LEVEL_LOW;
        }
        else
        {
            ch0_data = LEVEL_HIGH;
        }
    }

void loop(void)
{
  static uint64_t started = 0;
  if (millis() - started >= 10)
  {
       started = millis();    
        if (LEVEL_HIGH == ch4_data)
        {
            ch4_data = LEVEL_LOW;
        }
        else
        {
            ch4_data = LEVEL_HIGH;
        }
    }
    
//Maybe insert some kind of offset filter later on

//Add All the Data
s0.addValue( 0, CH0_OFFSET + ch0_data); //Tidal Volume Graph Data
s0.addValue( 1, CH1_OFFSET + ch1_data);
s0.addValue( 2, CH2_OFFSET + ch2_data);
s0.addValue( 3, CH3_OFFSET + ch3_data);

s1.addValue( 4, CH0_OFFSET + ch0_data); //Tidal Volume Graph Settings Data
s1.addValue( 5, CH1_OFFSET + ch1_data);
s1.addValue( 6, CH2_OFFSET + ch2_data);
s1.addValue( 7, CH3_OFFSET + ch3_data);

s2.addValue( 8, CH0_OFFSET + ch0_data); //Tidal Volume Diagnostics Data
s2.addValue( 9, CH1_OFFSET + ch1_data); 
s2.addValue(10, CH2_OFFSET + ch2_data);
s2.addValue(11, CH3_OFFSET + ch3_data);

s3.addValue(12, CH4_OFFSET + ch4_data); //Volume Flow Graph Data
s3.addValue(13, CH5_OFFSET + ch5_data);
s3.addValue(14, CH6_OFFSET + ch6_data);
s3.addValue(15, CH7_OFFSET + ch7_data);

s4.addValue(16, CH4_OFFSET + ch4_data); //Volume Flow Graph Settings Data
s4.addValue(17, CH5_OFFSET + ch5_data);
s4.addValue(18, CH6_OFFSET + ch6_data);
s4.addValue(19, CH7_OFFSET + ch7_data);

s5.addValue(20, CH4_OFFSET + ch4_data); //Volume Flow Diagnostics Data
s5.addValue(21, CH5_OFFSET + ch5_data);
s5.addValue(22, CH6_OFFSET + ch6_data);
s5.addValue(23, CH7_OFFSET + ch7_data);
