#include "usbh_cdc.h"
#include "defines.h"
#include "log.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


#define TIMER_START     static uint startFrame = 0; \
                        static uint timeTicks = 0;     \
                        uint start = TIM2->CNT;   \
                        static int counter = 0;    \
                        counter++;


#define TIMER_END       uint end = TIM2->CNT; \
                        timeTicks += (end - start);    \
                        if(TIME_MS - startFrame >= 1000)    \
                        {   \
                            LOG_WRITE("%s время обработки %d ms, %d раз", __FUNCTION__, timeTicks / 90000, counter); \
                            timeTicks = 0; \
                            startFrame = TIME_MS;   \
                            Timer::StartMultiMeasurement(); \
                            counter = 0;    \
                        }

#define USBH_ADDRESS_DEFAULT                     0x00U

extern "C" {


    extern USBH_StatusTypeDef SetLineCoding(USBH_HandleTypeDef *phost, CDC_LineCodingTypeDef *linecoding);
    extern USBH_StatusTypeDef GetLineCoding(USBH_HandleTypeDef *phost, CDC_LineCodingTypeDef *linecoding);
    extern void CDC_ProcessTransmission(USBH_HandleTypeDef *phost);
    extern USBH_StatusTypeDef USBH_HandleEnum(USBH_HandleTypeDef *phost);
    extern USBH_StatusTypeDef DeInitStateMachine(USBH_HandleTypeDef *phost);
    extern void HCD_Port_IRQHandler(HCD_HandleTypeDef *hhcd);
    extern void HCD_HC_IN_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum);
    extern void HCD_HC_OUT_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum);
    extern void HCD_RXQLVL_IRQHandler(HCD_HandleTypeDef *hhcd);

    void CDC_ProcessReception(USBH_HandleTypeDef *phost)
    {
        CDC_HandleTypeDef *CDC_Handle = (CDC_HandleTypeDef *)phost->pActiveClass->pData;
        USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
        uint32_t length;

        switch(CDC_Handle->data_rx_state)
        {

        case CDC_RECEIVE_DATA:

            USBH_BulkReceiveData(phost,
                                 CDC_Handle->pRxData,
                                 CDC_Handle->DataItf.InEpSize,
                                 CDC_Handle->DataItf.InPipe);

            CDC_Handle->data_rx_state = CDC_RECEIVE_DATA_WAIT;

            break;

        case CDC_RECEIVE_DATA_WAIT:

            URB_Status = USBH_LL_GetURBState(phost, CDC_Handle->DataItf.InPipe);

            /*Check the status done for reception*/
            if(URB_Status == USBH_URB_DONE)
            {
                length = USBH_LL_GetLastXferSize(phost, CDC_Handle->DataItf.InPipe);

                if(((CDC_Handle->RxDataLength - length) > 0U) && (length > CDC_Handle->DataItf.InEpSize))
                {
                    CDC_Handle->RxDataLength -= length;
                    CDC_Handle->pRxData += length;
                    CDC_Handle->data_rx_state = CDC_RECEIVE_DATA;
                }
                else
                {
                    CDC_Handle->data_rx_state = CDC_IDLE;
                    USBH_CDC_ReceiveCallback(phost);
                }
            }
            break;

        case CDC_IDLE:
        case CDC_SEND_DATA:
        case CDC_SEND_DATA_WAIT:
        default:
            break;
        }
    }

    USBH_StatusTypeDef USBH_CDC_Process(USBH_HandleTypeDef *phost)
    {
        USBH_StatusTypeDef status = USBH_BUSY;
        USBH_StatusTypeDef req_status = USBH_OK;
        CDC_HandleTypeDef *CDC_Handle = (CDC_HandleTypeDef *)phost->pActiveClass->pData;

        switch(CDC_Handle->state)
        {

        case CDC_IDLE_STATE:
            status = USBH_OK;
            break;

        case CDC_SET_LINE_CODING_STATE:
            req_status = SetLineCoding(phost, CDC_Handle->pUserLineCoding);

            if(req_status == USBH_OK)
            {
                CDC_Handle->state = CDC_GET_LAST_LINE_CODING_STATE;
            }

            else
            {
                if(req_status != USBH_BUSY)
                {
                    CDC_Handle->state = CDC_ERROR_STATE;
                }
            }
            break;


        case CDC_GET_LAST_LINE_CODING_STATE:
            req_status = GetLineCoding(phost, &(CDC_Handle->LineCoding));

            if(req_status == USBH_OK)
            {
                CDC_Handle->state = CDC_IDLE_STATE;

                if((CDC_Handle->LineCoding.b.bCharFormat == CDC_Handle->pUserLineCoding->b.bCharFormat) &&
                    (CDC_Handle->LineCoding.b.bDataBits == CDC_Handle->pUserLineCoding->b.bDataBits) &&
                   (CDC_Handle->LineCoding.b.bParityType == CDC_Handle->pUserLineCoding->b.bParityType) &&
                   (CDC_Handle->LineCoding.b.dwDTERate == CDC_Handle->pUserLineCoding->b.dwDTERate))
                {
                    USBH_CDC_LineCodingChanged(phost);
                }
            }
            else
            {
                if(req_status != USBH_BUSY)
                {
                    CDC_Handle->state = CDC_ERROR_STATE;
                }
            }
            break;

        case CDC_TRANSFER_DATA:
            CDC_ProcessTransmission(phost);
            CDC_ProcessReception(phost);
            break;

        case CDC_ERROR_STATE:
            req_status = USBH_ClrFeature(phost, 0x00U);

            if(req_status == USBH_OK)
            {
                /*Change the state to waiting*/
                CDC_Handle->state = CDC_IDLE_STATE;
            }
            break;

        default:
            break;

        }

        return status;
    }


    /**
  * @brief  USBH_Process
  *         Background process of the USB Core.
  * @param  phost: Host Handle
  * @retval USBH Status
  */
    USBH_StatusTypeDef  USBH_Process(USBH_HandleTypeDef *phost)
    {
        __IO USBH_StatusTypeDef status = USBH_FAIL;
        uint8_t idx = 0U;

        /* check for Host pending port disconnect event */
        if(phost->device.is_disconnected == 1U)
        {
            phost->gState = HOST_DEV_DISCONNECTED;
        }

        switch(phost->gState)
        {
        case HOST_IDLE:

            if(phost->device.is_connected)
            {
                USBH_UsrLog("USB Device Connected");

                /* Wait for 200 ms after connection */
                phost->gState = HOST_DEV_WAIT_FOR_ATTACHMENT;
                USBH_Delay(200U);
                USBH_LL_ResetPort(phost);

                /* Make sure to start with Default address */
                phost->device.address = USBH_ADDRESS_DEFAULT;
                phost->Timeout = 0U;

#if (USBH_USE_OS == 1U)
                phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
                (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
                (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            }
            break;

        case HOST_DEV_WAIT_FOR_ATTACHMENT: /* Wait for Port Enabled */

            if(phost->device.PortEnabled == 1U)
            {
                USBH_UsrLog("USB Device Reset Completed");
                phost->device.RstCnt = 0U;
                phost->gState = HOST_DEV_ATTACHED;
            }
            else
            {
                if(phost->Timeout > USBH_DEV_RESET_TIMEOUT)
                {
                    phost->device.RstCnt++;
                    if(phost->device.RstCnt > 3U)
                    {
                        /* Buggy Device can't complete reset */
                        USBH_UsrLog("USB Reset Failed, Please unplug the Device.");
                        phost->gState = HOST_ABORT_STATE;
                    }
                    else
                    {
                        phost->gState = HOST_IDLE;
                    }
                }
                else
                {
                    phost->Timeout += 10U;
                    USBH_Delay(10U);
                }
            }
#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_DEV_ATTACHED:

            if(phost->pUser != NULL)
            {
                phost->pUser(phost, HOST_USER_CONNECTION);
            }

            /* Wait for 100 ms after Reset */
            USBH_Delay(100U);

            phost->device.speed = (uint8_t)USBH_LL_GetSpeed(phost);

            phost->gState = HOST_ENUMERATION;

            phost->Control.pipe_out = USBH_AllocPipe(phost, 0x00U);
            phost->Control.pipe_in = USBH_AllocPipe(phost, 0x80U);

            /* Open Control pipes */
            USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80U,
                          phost->device.address, phost->device.speed,
                          USBH_EP_CONTROL, (uint16_t)phost->Control.pipe_size);

            /* Open Control pipes */
            USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00U,
                          phost->device.address, phost->device.speed,
                          USBH_EP_CONTROL, (uint16_t)phost->Control.pipe_size);

#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_ENUMERATION:
            /* Check for enumeration status */
            status = USBH_HandleEnum(phost);
            if(status == USBH_OK)
            {
                /* The function shall return USBH_OK when full enumeration is complete */
                USBH_UsrLog("Enumeration done.");

                phost->device.current_interface = 0U;

                if(phost->device.DevDesc.bNumConfigurations == 1U)
                {
                    USBH_UsrLog("This device has only 1 configuration.");
                    phost->gState = HOST_SET_CONFIGURATION;
                }
                else
                {
                    phost->gState = HOST_INPUT;
                }
#if (USBH_USE_OS == 1U)
                phost->os_msg = (uint32_t)USBH_STATE_CHANGED_EVENT;
#if (osCMSIS < 0x20000U)
                (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
                (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            }
            break;

        case HOST_INPUT:
        {
            /* user callback for end of device basic enumeration */
            if(phost->pUser != NULL)
            {
                phost->pUser(phost, HOST_USER_SELECT_CONFIGURATION);
                phost->gState = HOST_SET_CONFIGURATION;

#if (USBH_USE_OS == 1U)
                phost->os_msg = (uint32_t)USBH_STATE_CHANGED_EVENT;
#if (osCMSIS < 0x20000U)
                (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
                (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            }
        }
        break;

        case HOST_SET_CONFIGURATION:
            /* set configuration */
            if(USBH_SetCfg(phost, (uint16_t)phost->device.CfgDesc.bConfigurationValue) == USBH_OK)
            {
                phost->gState = HOST_SET_WAKEUP_FEATURE;
                USBH_UsrLog("Default configuration set.");
            }

#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case  HOST_SET_WAKEUP_FEATURE:

            if((phost->device.CfgDesc.bmAttributes) & (1U << 5))
            {
                if(USBH_SetFeature(phost, FEATURE_SELECTOR_REMOTEWAKEUP) == USBH_OK)
                {
                    USBH_UsrLog("Device remote wakeup enabled");
                    phost->gState = HOST_CHECK_CLASS;
                }
            }
            else
            {
                phost->gState = HOST_CHECK_CLASS;
            }

#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_CHECK_CLASS:

            if(phost->ClassNumber == 0U)
            {
                USBH_UsrLog("No Class has been registered.");
            }
            else
            {
                phost->pActiveClass = NULL;

                for(idx = 0U; idx < USBH_MAX_NUM_SUPPORTED_CLASS; idx++)
                {
                    if(phost->pClass[idx]->ClassCode == phost->device.CfgDesc.Itf_Desc[0].bInterfaceClass)
                    {
                        phost->pActiveClass = phost->pClass[idx];
                        break;
                    }
                }

                if(phost->pActiveClass != NULL)
                {
                    if(phost->pActiveClass->Init(phost) == USBH_OK)
                    {
                        phost->gState = HOST_CLASS_REQUEST;
                        USBH_UsrLog("%s class started.", phost->pActiveClass->Name);

                        /* Inform user that a class has been activated */
                        phost->pUser(phost, HOST_USER_CLASS_SELECTED);
                    }
                    else
                    {
                        phost->gState = HOST_ABORT_STATE;
                        USBH_UsrLog("Device not supporting %s class.", phost->pActiveClass->Name);
                    }
                }
                else
                {
                    phost->gState = HOST_ABORT_STATE;
                    USBH_UsrLog("No registered class for this device.");
                }
            }

#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_STATE_CHANGED_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_CLASS_REQUEST:
            /* process class standard control requests state machine */
            if(phost->pActiveClass != NULL)
            {
                status = phost->pActiveClass->Requests(phost);

                if(status == USBH_OK)
                {
                    phost->gState = HOST_CLASS;
                }
                else if(status == USBH_FAIL)
                {
                    phost->gState = HOST_ABORT_STATE;
                    USBH_ErrLog("Device not responding Please Unplug.");
                }
                else
                {
                    /* .. */
                }
            }
            else
            {
                phost->gState = HOST_ABORT_STATE;
                USBH_ErrLog("Invalid Class Driver.");
            }
#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_STATE_CHANGED_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_CLASS:
            /* process class state machine */
            if(phost->pActiveClass != NULL)
            {
                phost->pActiveClass->BgndProcess(phost);
            }
            break;

        case HOST_DEV_DISCONNECTED:
            phost->device.is_disconnected = 0U;

            DeInitStateMachine(phost);

            /* Re-Initilaize Host for new Enumeration */
            if(phost->pActiveClass != NULL)
            {
                phost->pActiveClass->DeInit(phost);
                phost->pActiveClass = NULL;
            }

            if(phost->pUser != NULL)
            {
                phost->pUser(phost, HOST_USER_DISCONNECTION);
            }
            USBH_UsrLog("USB Device disconnected");

            if(phost->device.is_ReEnumerated == 1U)
            {
                phost->device.is_ReEnumerated = 0U;

                /* Start the host and re-enable Vbus */
                USBH_Start(phost);
            }
            else
            {
                /* Device Disconnection Completed, start USB Driver */
                USBH_LL_Start(phost);
            }

#if (USBH_USE_OS == 1U)
            phost->os_msg = (uint32_t)USBH_PORT_EVENT;
#if (osCMSIS < 0x20000U)
            (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
            (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
            break;

        case HOST_ABORT_STATE:
        case HOST_DETECT_DEVICE_SPEED:
        case HOST_SUSPENDED:
        default:
            break;
        }

        return USBH_OK;
    }


    /**
  * @brief  Handle HCD interrupt request.
  * @param  hhcd HCD handle
  * @retval None
  */
    void HAL_HCD_IRQHandler(HCD_HandleTypeDef *hhcd)
    {
        USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;
        uint32_t USBx_BASE = (uint32_t)USBx;
        uint32_t i, interrupt;

        /* Ensure that we are in device mode */
        if(USB_GetMode(hhcd->Instance) == USB_OTG_MODE_HOST)
        {
            /* Avoid spurious interrupt */
            if(__HAL_HCD_IS_INVALID_INTERRUPT(hhcd))
            {
                return;
            }

            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
            {
                /* Incorrect mode, acknowledge the interrupt */
                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
            }

            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_IISOIXFR))
            {
                /* Incorrect mode, acknowledge the interrupt */
                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_IISOIXFR);
            }

            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_PTXFE))
            {
                /* Incorrect mode, acknowledge the interrupt */
                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_PTXFE);
            }

            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_MMIS))
            {
                /* Incorrect mode, acknowledge the interrupt */
                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_MMIS);
            }

            /* Handle Host Disconnect Interrupts */
            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_DISCINT))
            {
                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_DISCINT);

                if((USBx_HPRT0 & USB_OTG_HPRT_PCSTS) == 0U)
                {
                    /* Handle Host Port Disconnect Interrupt */
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)
                    hhcd->DisconnectCallback(hhcd);
#else
                    HAL_HCD_Disconnect_Callback(hhcd);
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

                    (void)USB_InitFSLSPClkSel(hhcd->Instance, HCFG_48_MHZ);
                }
            }

            /* Handle Host Port Interrupts */
            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_HPRTINT))
            {
                HCD_Port_IRQHandler(hhcd);
            }

            /* Handle Host SOF Interrupt */
            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_SOF))
            {
#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)
                hhcd->SOFCallback(hhcd);
#else
                HAL_HCD_SOF_Callback(hhcd);
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */

                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_SOF);
            }

            /* Handle Host channel Interrupt */
            if(__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_HCINT))
            {
                static uint startFrame = 0;
                static int counter = 0;
                static uint time = 0;
                counter++;

                uint start = TIME_MS;

                interrupt = USB_HC_ReadInterrupt(hhcd->Instance);

                for(i = 0U; i < hhcd->Init.Host_channels; i++)
                {
                    if((interrupt & (1UL << (i & 0xFU))) != 0U)
                    {
                        if((USBx_HC(i)->HCCHAR & USB_OTG_HCCHAR_EPDIR) == USB_OTG_HCCHAR_EPDIR)
                        {
                            HCD_HC_IN_IRQHandler(hhcd, (uint8_t)i);
                        }
                        else
                        {
                            HCD_HC_OUT_IRQHandler(hhcd, (uint8_t)i);
                        }
                    }
                }

                time += (TIME_MS - start);

                if(TIME_MS - startFrame >= 1000)
                {
                    LOG_WRITE("флаг %d раз, %d мс", counter, time);
                    counter = 0;
                    startFrame = TIME_MS;
                    time = 0;
                }

                __HAL_HCD_CLEAR_FLAG(hhcd, USB_OTG_GINTSTS_HCINT);
            }

            /* Handle Rx Queue Level Interrupts */
            if((__HAL_HCD_GET_FLAG(hhcd, USB_OTG_GINTSTS_RXFLVL)) != 0U)
            {
                USB_MASK_INTERRUPT(hhcd->Instance, USB_OTG_GINTSTS_RXFLVL);

                HCD_RXQLVL_IRQHandler(hhcd);

                USB_UNMASK_INTERRUPT(hhcd->Instance, USB_OTG_GINTSTS_RXFLVL);
            }
        }
    }


    void HCD_HC_IN_IRQHandler(HCD_HandleTypeDef *hhcd, uint8_t chnum)
    {
        TIMER_START;

        USB_OTG_GlobalTypeDef *USBx = hhcd->Instance;
        uint32_t USBx_BASE = (uint32_t)USBx;
        uint32_t ch_num = (uint32_t)chnum;

        uint32_t tmpreg;

        if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_AHBERR) == USB_OTG_HCINT_AHBERR)
        {
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_AHBERR);
            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_BBERR) == USB_OTG_HCINT_BBERR)
        {
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_BBERR);
            hhcd->hc[ch_num].state = HC_BBLERR;
            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
            (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_ACK) == USB_OTG_HCINT_ACK)
        {
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_ACK);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_STALL) == USB_OTG_HCINT_STALL)
        {
            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
            hhcd->hc[ch_num].state = HC_STALL;
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_STALL);
            (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_DTERR) == USB_OTG_HCINT_DTERR)
        {
            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
            (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);
            hhcd->hc[ch_num].state = HC_DATATGLERR;
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_DTERR);
        }
        else
        {
            /* ... */
        }

        if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_FRMOR) == USB_OTG_HCINT_FRMOR)
        {

            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
            (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_FRMOR);
        }      
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_XFRC) == USB_OTG_HCINT_XFRC)
        {
            if(hhcd->Init.dma_enable != 0U)
            {
                hhcd->hc[ch_num].xfer_count = hhcd->hc[ch_num].xfer_len - \
                    (USBx_HC(ch_num)->HCTSIZ & USB_OTG_HCTSIZ_XFRSIZ);
            }

            hhcd->hc[ch_num].state = HC_XFRC;
            hhcd->hc[ch_num].ErrCnt = 0U;
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_XFRC);

            if((hhcd->hc[ch_num].ep_type == EP_TYPE_CTRL) ||
                (hhcd->hc[ch_num].ep_type == EP_TYPE_BULK))
            {
                __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
                (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
                __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);
            }
            else if(hhcd->hc[ch_num].ep_type == EP_TYPE_INTR)
            {
                USBx_HC(ch_num)->HCCHAR |= USB_OTG_HCCHAR_ODDFRM;
                hhcd->hc[ch_num].urb_state = URB_DONE;

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)
                hhcd->HC_NotifyURBChangeCallback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);
#else
                HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
            }
            else if(hhcd->hc[ch_num].ep_type == EP_TYPE_ISOC)
            {
                hhcd->hc[ch_num].urb_state = URB_DONE;
                hhcd->hc[ch_num].toggle_in ^= 1U;

#if (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)
                hhcd->HC_NotifyURBChangeCallback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);
#else
                HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);
#endif /* USE_HAL_HCD_REGISTER_CALLBACKS */
            }
            else
            {
                /* ... */
            }
            hhcd->hc[ch_num].toggle_in ^= 1U;
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_CHH) == USB_OTG_HCINT_CHH)
        {
            __HAL_HCD_MASK_HALT_HC_INT(ch_num);

            if(hhcd->hc[ch_num].state == HC_XFRC)
            {
                hhcd->hc[ch_num].urb_state = URB_DONE;
            }
            else if(hhcd->hc[ch_num].state == HC_STALL)
            {
                hhcd->hc[ch_num].urb_state = URB_STALL;
            }
            else if((hhcd->hc[ch_num].state == HC_XACTERR) ||
                (hhcd->hc[ch_num].state == HC_DATATGLERR))
            {
                hhcd->hc[ch_num].ErrCnt++;
                if(hhcd->hc[ch_num].ErrCnt > 3U)
                {
                    hhcd->hc[ch_num].ErrCnt = 0U;
                    hhcd->hc[ch_num].urb_state = URB_ERROR;
                }
                else
                {
                    hhcd->hc[ch_num].urb_state = URB_NOTREADY;
                }

                /* re-activate the channel  */
                tmpreg = USBx_HC(ch_num)->HCCHAR;
                tmpreg &= ~USB_OTG_HCCHAR_CHDIS;
                tmpreg |= USB_OTG_HCCHAR_CHENA;
                USBx_HC(ch_num)->HCCHAR = tmpreg;
            }
            else if(hhcd->hc[ch_num].state == HC_NAK)
            {
                hhcd->hc[ch_num].urb_state = URB_NOTREADY;
                /* re-activate the channel  */
                tmpreg = USBx_HC(ch_num)->HCCHAR;
                tmpreg &= ~USB_OTG_HCCHAR_CHDIS;
                tmpreg |= USB_OTG_HCCHAR_CHENA;
                USBx_HC(ch_num)->HCCHAR = tmpreg;
            }
            else if(hhcd->hc[ch_num].state == HC_BBLERR)
            {
                hhcd->hc[ch_num].ErrCnt++;
                hhcd->hc[ch_num].urb_state = URB_ERROR;
            }
            else
            {
                /* ... */
            }

            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_CHH);
            HAL_HCD_HC_NotifyURBChange_Callback(hhcd, (uint8_t)ch_num, hhcd->hc[ch_num].urb_state);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_TXERR) == USB_OTG_HCINT_TXERR)
        {
            __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
            hhcd->hc[ch_num].ErrCnt++;
            hhcd->hc[ch_num].state = HC_XACTERR;
            (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_TXERR);
        }
        else if((USBx_HC(ch_num)->HCINT & USB_OTG_HCINT_NAK) == USB_OTG_HCINT_NAK)
        {
            if(hhcd->hc[ch_num].ep_type == EP_TYPE_INTR)
            {
                hhcd->hc[ch_num].ErrCnt = 0U;
                __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
                (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
            }
            else if((hhcd->hc[ch_num].ep_type == EP_TYPE_CTRL) ||
                (hhcd->hc[ch_num].ep_type == EP_TYPE_BULK))
            {
                hhcd->hc[ch_num].ErrCnt = 0U;

                if(hhcd->Init.dma_enable == 0U)
                {
                    hhcd->hc[ch_num].state = HC_NAK;
                    __HAL_HCD_UNMASK_HALT_HC_INT(ch_num);
                    (void)USB_HC_Halt(hhcd->Instance, (uint8_t)ch_num);
                }
            }
            else
            {
                /* ... */
            }
            __HAL_HCD_CLEAR_HC_INT(ch_num, USB_OTG_HCINT_NAK);
        }
        else
        {
            /* ... */
        }

        TIMER_END;
    }

}
