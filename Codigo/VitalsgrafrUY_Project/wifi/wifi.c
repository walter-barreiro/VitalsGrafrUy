/**
 * @file wifi.c
 * @brief Vitalsgrafr UY - Proyecto Sistemas Embebidos 2020 - Fing
 * Este módulo se encarga de inicializar el Boosterpack CC3100 Wifi para la comunicación con la interfaz gráfica.
 *
 * @paragraph
 * Inicializa el Boosterpack CC3100 como Station, se conecta al Access Point inidicado
 * y a la interfaz Matlab mediante TCP/sockets en puerto 60001.
 * Se encarga de enviar y recibir los datos desde y hacia la interfaz gráfica.
 *
 * @author Diego Alonso German Minetti Walter Barreiro
 * @version 1.1
 * @date 29 de Mayo de 2020
 *
 * wifi.c
 *
 */

/* System Includes*/
#include <msp432.h>
#include "wifi.h"
#include "simplelink.h"
#include "sl_common.h"
#include "cli_uart.h"

/* DriverLib Includes */
#include "driverlib.h"

/*
 * Values for below macros shall be modified per the access-point's (AP) properties
 * SimpleLink device will connect to following AP when the application is executed
 */
#define SSID_NAME       "TP-LINK_B450"       /* Access point name to connect to. */
//#define SSID_NAME       "dominguez"
//#define SSID_NAME       "EL ANCLA"
//#define SSID_NAME       "moruli2"
//#define SSID_NAME       "Rosa Moll"
#define SEC_TYPE       SL_SEC_TYPE_WPA     /* Security type of the Access piont */
#define PASSKEY         "miba1234"   /* Password in case of secure AP */
//#define PASSKEY          "22044636"
//#define PASSKEY          "DISTRITOCERO"
//#define PASSKEY          "morulisimos2498"
//#define PASSKEY          "23561170"
#define PASSKEY_LEN     pal_Strlen(PASSKEY)      /* Password length in case of secure AP */


//#define IP_ADDR         0xc0a8012f     //192.168.1.47
//#define IP_ADDR         0xc0a80130     //192.168.1.48
//#define IP_ADDR         0xc0a80064     //192.168.0.100
//#define IP_ADDR         0xc0a80068     //192.168.0.104
#define IP_ADDR         0xc0a80067     //192.168.0.103
//#define IP_ADDR         0xc0a80069     //192.168.0.105
//#define IP_ADDR         0xc0a80072       //192.168.0.114
//#define IP_ADDR           0xc0a8006A      //192.168.0.106
//#define IP_ADDR           0xc0a80135       //192.168.1.53
#define PORT_NUM        60001            /* Port number to be used */
#define SERVERNAME     "VitalsGrafr UY Interfaz"

#define NO_OF_PACKETS           1
#define NO_OF_TRIES             100000

#define TRIES                   15000000
#define APPLICATION_VERSION "1.0.0"

#define SL_STOP_TIMEOUT        0xFF

#define SMALL_BUF           32

//#define MAX_SEND_BUF_SIZE   512
//#define MAX_RCV_SIZE   1024

/* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap w/ host-driver's error codes */
    TCP_SEND_ERROR = DEVICE_NOT_IN_STATION_MODE -1,
    TCP_RECV_ERROR = TCP_SEND_ERROR -1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

//#define min(X,Y) ((X) < (Y) ? (X) : (Y))


/*
 * GLOBAL VARIABLES -- Start
 */

_u32  g_Status = 0;
struct{
    _u8 HostName[SMALL_BUF];
    _u8 CityName[SMALL_BUF];

    _u32 DestinationIP;
    _i16 SockID;
}g_AppData;

/* Port mapper configuration register */
const uint8_t port_mapping[] =
{
    //Port P2:
    PM_TA0CCR1A, PM_TA0CCR2A, PM_TA0CCR3A, PM_NONE, PM_TA1CCR1A, PM_NONE, PM_NONE, PM_NONE
};


static _u8 SendBuf[MAX_SEND_BUF];
static _u8 RecvBuf[MAX_RECV_BUF];


_i16 Sock;
             
/*
 * STATIC FUNCTION DEFINITIONS -- Start
 */
static _i32 establishConnectionWithAP();
static _i32 configureSimpleLinkToDefaultState();
static _i32 initializeAppVariables();
static void displayBanner();
static _i32 BsdTcpClient(_u16 Port);






///*********************************************************************
//Inicializa y abre TCP, devuele la direccion del socket abierto.
_i16 InitWifi()
{
    _i32 retVal = -1;
    _i32 EXITOSA = 100;

    /* Stop WDT and initialize the system-clock of the MCU */

   // initClk();

    /* Confinguring P1.1 & P1.4 as an input and enabling interrupts */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    Interrupt_enableMaster();

    /* Configure command line interface */
    CLI_Configure();

    CLI_Write((_u8 *)" Inicializando Wifi...");

    /*
     * Following function configures the device to default state by cleaning
     * the persistent settings stored in NVMEM (viz. connection profiles &
     * policies, power policy etc)
     *
     * Applications may choose to skip this step if the developer is sure
     * that the device is in its default state at start of application
     *
     * Note that all profiles and persistent settings that were done on the
     * device will be lost
     */
    retVal = configureSimpleLinkToDefaultState();
    if(retVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == retVal)
            CLI_Write(" Dispositivo NO en estado por defecto \n\r");

        //LOOP_FOREVER();
        EXITOSA = -1;
    }

    CLI_Write(" Dispositivo en estado por defecto \n\r");

    /*
     * Assumption is that the device is configured in station mode already
     * and it is in its default state
     */
    retVal = sl_Start(0, 0, 0);
    if ((retVal < 0) ||
        (ROLE_STA != retVal) )
    {
        CLI_Write(" Fallo al iniciar dispositivo \n\r");
       // LOOP_FOREVER();
        EXITOSA = -1;
    }

    CLI_Write(" Dispositivo inicializado como STATION \n\r");

    /* Connecting to WLAN AP */
    retVal = establishConnectionWithAP();
    if(retVal < 0)
    {
        CLI_Write(" Dispositivo NO conectado con Access Point  \n\r");
        //LOOP_FOREVER();
        EXITOSA = -1;
    } else{

    CLI_Write(" Conexión establecida con ");
    CLI_Write(SSID_NAME);
    CLI_Write("\n\r");
    }
    //*****************************************************************************
    CLI_Write(" Estableciendo comunicación con servidor TCP...\n\r");
        /*Before proceeding, please make sure to have a server waiting on PORT_NUM*/
        Sock = BsdTcpClient(PORT_NUM);
        if(Sock < 0){
            CLI_Write(" Conexión con servidor fallida TCP \n\r");
            EXITOSA = -1;
        }
            else
            CLI_Write(" Conectado con servidor TCP VitalsGrafr UY \n\r");

    return EXITOSA;
}


//**********************************FUNCIONES WIFI**********************

_i32 WifiClose(){
  _i32 retVal = -1;

    retVal = sl_Close(Sock);
    ASSERT_ON_ERROR(retVal);

  // Stop the CC3100 device 
    retVal = sl_Stop(SL_STOP_TIMEOUT);
  //  if(retVal < 0)

  
    return SUCCESS;
}
  

/////////////Manda mensaje, TamBata tiene que ser menor a 1490 bytes.

_i32 SendDataWifi(_u8* pDataS, _i16 TamData){
  
//CONSULTAR SI EL BUFFER ESTA LLENO O VACIO
  
  _i16 Status=0;
  _i32 i=0;
  
    do{
	SendBuf[i] = pDataS[i];
    }while(i++ < TamData);
       
    Status = sl_Send(Sock, SendBuf, TamData, 0 );
    if( Status<=0)
    {
            CLI_Write(" [TCP Client] Dato no enviado, error \n\r");
            Status = sl_Close(Sock);
            ASSERT_ON_ERROR(TCP_SEND_ERROR);
    }  
    return SUCCESS;
}
//****RETORNA LA CANTIDAD DE DATOS RECIBIDOS o ERROR

_i32 RecvDataWifi(uint32_t* pDataR){
     
     _i16 Status=-1,count=0;
     while((Status<=0)&&(count<=TRIES))
     {
          Status = sl_Recv(Sock, RecvBuf, MAX_RECV_BUF, 0);
          count++;
     }

     
  
     if( Status <= 0 )
        {
            CLI_Write("Datos no recibidos \n\r");
            //Status = sl_Close(Sock);
            ASSERT_ON_ERROR(TCP_SEND_ERROR);
        }
     CLI_Write("[TCP Server] Datos recibidos correctamente\n\r");
     
        
      if (Status>0)
      {
        int i=0;
        do
        {
            pDataR[i] = RecvBuf[i];
        }while(i++<Status);

      }
      return Status;
}
//****Devuelve la direccion del socket a la que se mandaran datos

static _i32 BsdTcpClient(_u16 Port)   //manda mensajes de error en sock<0 y status<0
{
    SlSockAddrIn_t  Addr;

  
    _u16          AddrSize = 0;
    _i16          Status = 0;
    


    Addr.sin_family = SL_AF_INET;                       // direccion del socket
    Addr.sin_port = sl_Htons((_u16)Port);
    Addr.sin_addr.s_addr = sl_Htonl((_u32)IP_ADDR);
    AddrSize = sizeof(SlSockAddrIn_t);

    Sock = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( Sock < 0 )
    {
        CLI_Write(" [TCP Client] Error al crear sockets \n\r");    //error socket
        ASSERT_ON_ERROR(Sock);
    }

    sl_Bind(Sock, (SlSockAddr_t *)&Addr, AddrSize);
    Status = sl_Connect(Sock, ( SlSockAddr_t *)&Addr, AddrSize);
    if( Status < 0 )
    {
        sl_Close(Sock);
        CLI_Write(" [TCP Client]  Error en la conexión TCP \n\r");  //error conexion TCP
        ASSERT_ON_ERROR(Status);

    }
    ////Set non-blocking*////
    //CLI_Write(" [TCP Client]  Error en la conexión TCP \n\r");
    SlSockNonblocking_t enableOption;
    enableOption.NonblockingEnabled = 1;
    sl_SetSockOpt(Sock,SL_SOL_SOCKET,SL_SO_NONBLOCKING, (_u8 *)&enableOption,sizeof(enableOption));

 
    return SUCCESS;
}


/*!
    \brief This function configure the SimpleLink device in its default state. It:
           - Sets the mode to STATION
           - Configures connection policy to Auto and AutoSmartConfig
           - Deletes all the stored profiles
           - Enables DHCP
           - Disables Scan policy
           - Sets Tx power to maximum
           - Sets power policy to normal
           - Unregisters mDNS services
           - Remove all filters

    \param[in]      none

    \return         On success, zero is returned. On error, negative is returned
*/
static _i32 configureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    _u8           val = 1;
    _u8           configOpt = 0;
    _u8           configLen = 0;
    _u8           power = 0;

    _i32          retVal = -1;
    _i32          mode = -1;

    mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(mode);

    /* If the device is not in station-mode, try configuring it in station-mode */
    if (ROLE_STA != mode)
    {
        if (ROLE_AP == mode)
        {
            /* If the device is in AP mode, we need to wait for this event before doing anything */
            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
        }

        /* Switch to STA role and restart */
        retVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(retVal);

        /* Check if the device is in station again */
        if (ROLE_STA != retVal)
        {
            /* We don't want to proceed if the device is not coming up in station-mode */
            ASSERT_ON_ERROR(DEVICE_NOT_IN_STATION_MODE);
        }
    }

    /* Get the device's version-information */
    configOpt = SL_DEVICE_GENERAL_VERSION;
    configLen = sizeof(ver);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (_u8 *)(&ver));
    ASSERT_ON_ERROR(retVal);

    /* Set connection policy to Auto + SmartConfig (Device's default connection policy) */
    retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove all profiles */
    retVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(retVal);

    /*
     * Device in station-mode. Disconnect previous connection if any
     * The function returns 0 if 'Disconnected done', negative number if already disconnected
     * Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
     */
    retVal = sl_WlanDisconnect();
    if(0 == retVal)
    {
        /* Wait */
        while(IS_CONNECTED(g_Status)) { _SlNonOsMainLoopTask(); }
    }

    /* Enable DHCP client*/
    retVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);
    ASSERT_ON_ERROR(retVal);

    /* Disable scan */
    configOpt = SL_SCAN_POLICY(0);
    retVal = sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Set Tx power level for station mode
       Number between 0-15, as dB offset from max power - 0 will set maximum power */
    power = 0;
    retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (_u8 *)&power);
    ASSERT_ON_ERROR(retVal);

    /* Set PM policy to normal */
    retVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Unregister mDNS services */
    retVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove  all 64 filters (8*8) */
    pal_Memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    retVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(retVal);

    retVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(retVal);

    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);

    return retVal; /* Success */
}

/*!
    \brief Connecting to a WLAN Access point

    This function connects to the required AP (SSID_NAME).
    The function will return once we are connected and have acquired IP address

    \param[in]  None

    \return     0 on success, negative error-code on error

    \note

    \warning    If the WLAN connection fails or we don't acquire an IP address,
                We will be stuck in this function forever.
*/
static _i32 establishConnectionWithAP()
{
    SlSecParams_t secParams = {0};
    _i32 retVal = 0;

    secParams.Key = PASSKEY;
    secParams.KeyLen = PASSKEY_LEN;
    secParams.Type = SEC_TYPE;

    retVal = sl_WlanConnect(SSID_NAME, pal_Strlen(SSID_NAME), 0, &secParams, 0);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
  //  while((!IS_CONNECTED(g_Status)) || (!IS_IP_ACQUIRED(g_Status))) { _SlNonOsMainLoopTask(); }

    return SUCCESS;
}

/*!
    \brief This function initializes the application variables

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
static _i32 initializeAppVariables()
{
    g_Status = 0;
    pal_Memset(&g_AppData, 0, sizeof(g_AppData));

    return SUCCESS;
}

/*!
    \brief This function displays the application's banner

    \param      None

    \return     None
*/
static void displayBanner()
{
    CLI_Write("\n\r\n\r");
    CLI_Write(" MSP432Uart - Version ");
    CLI_Write(APPLICATION_VERSION);
    CLI_Write("\n\r*******************************************************************************\n\r");
}
/*!
    \brief This function handles WLAN events

    \param[in]      pWlanEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(pWlanEvent == NULL)
        CLI_Write(" [WLAN EVENT] NULL Pointer Error \n\r");
    
    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);

            /*
             * Information about the connected AP (like name, MAC etc) will be
             * available in 'slWlanConnectAsyncResponse_t' - Applications
             * can use it if required
             *
             * slWlanConnectAsyncResponse_t *pEventData = NULL;
             * pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
             *
             */
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            /* If the user has initiated 'Disconnect' request, 'reason_code' is SL_USER_INITIATED_DISCONNECTION */
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                CLI_Write(" Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                CLI_Write(" Device disconnected from the AP on an ERROR..!! \n\r");
            }
        }
        break;

        default:
        {
            CLI_Write(" [WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*
    \brief This function handles events for IP address acquisition via DHCP
           indication

    \param[in]      pNetAppEvent is the event passed to the handler

    \return         None

    \note

    \warning
*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(pNetAppEvent == NULL)
        CLI_Write(" [NETAPP EVENT] NULL Pointer Error \n\r");
 
    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            /*
             * Information about the connected AP's IP, gateway, DNS etc
             * will be available in 'SlIpV4AcquiredAsync_t' - Applications
             * can use it if required
             *
             * SlIpV4AcquiredAsync_t *pEventData = NULL;
             * pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
             * <gateway_ip> = pEventData->gateway;
             *
             */
        }
        break;

        default:
        {
            CLI_Write(" [NETAPP EVENT] Unexpected event \n\r");
        }
        break;
    }
}

/*!
    \brief This function handles callback for the HTTP server events

    \param[in]      pHttpEvent - Contains the relevant event information
    \param[in]      pHttpResponse - Should be filled by the user with the
                    relevant response information

    \return         None

    \note

    \warning
*/
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    /*
     * This application doesn't work with HTTP server - Hence these
     * events are not handled here
     */
    CLI_Write(" [HTTP EVENT] Unexpected event \n\r");
}

/*!
    \brief This function handles general error events indication

    \param[in]      pDevEvent is the event passed to the handler

    \return         None
*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    CLI_Write(" [GENERAL EVENT] \n\r");
}

/*!
    \brief This function handles socket events indication

    \param[in]      pSock is the event passed to the handler

    \return         None
*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    if(pSock == NULL)
        CLI_Write(" [SOCK EVENT] NULL Pointer Error \n\r");

    switch( pSock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT:
        {
            /*
            * TX Failed
            *
            * Information about the socket descriptor and status will be
            * available in 'SlSockEventData_t' - Applications can use it if
            * required
            *
            * SlSockEventData_t *pEventData = NULL;
            * pEventData = & pSock->EventData;
            */
            switch( pSock->EventData.status )
            {
                case SL_ECLOSE:
                    CLI_Write(" [SOCK EVENT] Close socket operation failed to transmit all queued packets\n\r");
                break;


                default:
                    CLI_Write(" [SOCK EVENT] Unexpected event \n\r");
                break;
            }
        }
        break;

        default:
            CLI_Write(" [SOCK EVENT] Unexpected event \n\r");
        break;
    }
}


/*
 * ASYNCHRONOUS EVENT HANDLERS -- End
 */


/*
 * Application's entry point
 */
