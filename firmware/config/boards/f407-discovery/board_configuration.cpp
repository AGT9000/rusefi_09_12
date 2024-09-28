/**
 * @file board_configuration.cpp
 */

#include "pch.h"
#include "hip9011_logic.h"
#include "defaults.h"

static void setEtb(){
	engineConfiguration->etbIo[0].controlPin = Gpio::E5;
	engineConfiguration->etbIo[0].directionPin1 = Gpio::E2;
	engineConfiguration->etbIo[0].disablePin = Gpio::E7;
	engineConfiguration->etb_use_two_wires = false;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_11;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_14;
	setPPSInputs(EFI_ADC_1, EFI_ADC_9);
	engineConfiguration->useFordRedundantPps = true;
	engineConfiguration->tpsSecondaryMaximum = 51.5;
	setPPSCalibration(1.41, 4.00, 0.70, 3.30);
	//setTPS1Calibration(100,809);
	//setToyota1NZFE();
}

static void setDefaultFrankensoStepperIdleParameters() {
	engineConfiguration->idle.stepperDirectionPin = Gpio::E10;
	engineConfiguration->idle.stepperStepPin = Gpio::E12;
	engineConfiguration->stepperEnablePin = Gpio::E14;
	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->idleStepperTotalSteps = 150;
}

static void setCanFrankensoDefaults() {
	engineConfiguration->canTxPin = Gpio::B6;
	engineConfiguration->canRxPin = Gpio::B12;
}

Gpio getWarningLedPin() {
    // open question if we need those LEDs at all? shall those be configurable?
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
#ifdef HW_NOT_COMMUNITY_FRANKENSO
  // not f407-discovery but f407-discovery while reusing board file
  return Gpio::D15;
#else
  // f407-discovery community board not frankenso
	return config->communityCommsLedPin;
#endif
}

Gpio getRunningLedPin() {
    // open question if we need those LEDs at all? shall those be configurable?
	return Gpio::Unassigned;
}

#if EFI_HIP_9011
static void setHip9011FrankensoPinout() {
	/**
	 * SPI on PB13/14/15
	 */
	//	engineConfiguration->hip9011CsPin = Gpio::D0; // rev 0.1

	engineConfiguration->isHip9011Enabled = true;
	engineConfiguration->hip9011PrescalerAndSDO = HIP_8MHZ_PRESCALER; // 8MHz chip
	engineConfiguration->is_enabled_spi_2 = true;
	// todo: convert this to rusEfi, hardware-independent enum
#if EFI_PROD_CODE
#ifdef EFI_HIP_CS_PIN
	engineConfiguration->hip9011CsPin = EFI_HIP_CS_PIN;
#else
	engineConfiguration->hip9011CsPin = Gpio::B0; // rev 0.4
#endif
	engineConfiguration->hip9011CsPinMode = OM_OPENDRAIN;

	engineConfiguration->hip9011IntHoldPin = Gpio::B11;
	engineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;

	engineConfiguration->spi2SckMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MosiMode = PO_OPENDRAIN; // 4
	engineConfiguration->spi2MisoMode = PO_PULLUP; // 32
#endif /* EFI_PROD_CODE */

	engineConfiguration->hip9011Gain = 1;

	if (!engineConfiguration->useTpicAdvancedMode) {
	    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
	}
}
#endif

#if EFI_ONBOARD_MEMS
static void configureAccelerometerPins() {
//	engineConfiguration->accelerometerCsPin = Gpio::E3; // we have a conflict with VVT output on Miata
// 	engineConfiguration->is_enabled_spi_1 = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	engineConfiguration->spi1mosiPin = Gpio::A7;
	engineConfiguration->spi1misoPin = Gpio::A6;
	engineConfiguration->spi1sckPin = Gpio::A5;
}
#endif // EFI_ONBOARD_MEMS

/**
 * @brief	Hardware board-specific default configuration (GPIO pins, ADC channels, SPI configs etc.)
 */
void setBoardDefaultConfiguration() {
	setDefaultFrankensoStepperIdleParameters();
	setCanFrankensoDefaults();

#ifndef HW_NOT_COMMUNITY_FRANKENSO
  // f407-discovery community board not frankenso
	config->communityCommsLedPin = Gpio::D15;  // blue LED on discovery
#endif

#if EFI_HIP_9011
	setHip9011FrankensoPinout();
#endif /* EFI_HIP_9011 */

	// set optional subsystem configs
#if EFI_ONBOARD_MEMS
	// this would override some values from above
	configureAccelerometerPins();
#endif /* EFI_ONBOARD_MEMS */

	setEtb();

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_12;
	//engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->clt.adcChannel = EFI_ADC_13;
	//engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->iat.adcChannel = EFI_ADC_10;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_15;

	engineConfiguration->fanPin = Gpio::E3;
	engineConfiguration->fuelPumpPin = Gpio::E6;


	//engineConfiguration->injectionPins[0] = Gpio::D12;
	engineConfiguration->injectionPins[0] = Gpio::Inj_Out1;
	engineConfiguration->injectionPins[1] = Gpio::D14;
	engineConfiguration->injectionPins[2] = Gpio::D13;
	engineConfiguration->injectionPins[3] = Gpio::D15;
	engineConfiguration->injectionPins[4] = Gpio::E9;
	engineConfiguration->injectionPins[5] = Gpio::E11;

	engineConfiguration->ignitionPins[0] = Gpio::D7;
	engineConfiguration->ignitionPins[1] = Gpio::B9;
	engineConfiguration->ignitionPins[2] = Gpio::A8;
	engineConfiguration->ignitionPins[3] = Gpio::D9;
	engineConfiguration->ignitionPins[4] = Gpio::D10;
	engineConfiguration->ignitionPins[5] = Gpio::E15;


	engineConfiguration->accelerometerSpiDevice = SPI_NONE;

/*

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->afr.hwChannel = EFI_ADC_14;

	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;

	engineConfiguration->triggerSimulatorPins[0] = Gpio::D3;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::D2;

	engineConfiguration->triggerInputPins[0] = Gpio::C6;
//	engineConfiguration->triggerInputPins[1] = Gpio::A5;

	// set this to SPI_DEVICE_3 to enable stimulation
	//engineConfiguration->digitalPotentiometerSpiDevice = SPI_DEVICE_3;
	engineConfiguration->digitalPotentiometerChipSelect[0] = Gpio::D7;
	engineConfiguration->digitalPotentiometerChipSelect[1] = Gpio::Unassigned;
	engineConfiguration->digitalPotentiometerChipSelect[2] = Gpio::D5;
	engineConfiguration->digitalPotentiometerChipSelect[3] = Gpio::Unassigned;

	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3; // please note that this pin is also SWO/SWD - Single Wire debug Output

	engineConfiguration->spi2mosiPin = Gpio::B15;
	engineConfiguration->spi2misoPin = Gpio::B14;
	engineConfiguration->spi2sckPin = Gpio::B13;

	engineConfiguration->spi3mosiPin = Gpio::B5;
	engineConfiguration->spi3misoPin = Gpio::B4;
	engineConfiguration->spi3sckPin = Gpio::B3;
*/


#if EFI_FILE_LOGGING
	setDefaultSdCardParameters();
#endif /* EFI_FILE_LOGGING */

	engineConfiguration->is_enabled_spi_1 = false;
	engineConfiguration->is_enabled_spi_2 = false;
	engineConfiguration->is_enabled_spi_3 = true;
}

// weak linkage
void boardInitHardware() {

static const struct mc33810_config mc33810 = {
	.spi_bus = &SPID3,
	.spi_config = {
		.circular = false,
		.end_cb = NULL,
		// todo: use engineConfiguration->mc33810_cs
		.ssport = GPIOC,
		.sspad = 5,
		.cr1 =
			SPI_CR1_16BIT_MODE |
			SPI_CR1_SSM |
			SPI_CR1_SSI |
			((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		.cr2 = SPI_CR2_16BIT_MODE
	},
	.direct_io = {
		/* injector drivers */
		[0] = {.port = GPIOA, .pad = 3},
		[1] = {.port = GPIOA, .pad = 4},
		[2] = {.port = nullptr, .pad = 0},
		[3] = {.port = nullptr, .pad = 0},
		/* ignition pre-drivers */
		[4] = {.port = GPIOA, .pad = 0},
		[5] = {.port = GPIOA, .pad = 1},
//GPGD mode is not supported yet, ignition mode does not support spi on/off commands
//	  so ignition signals should be directly driven
		[6] = {.port = GPIOD, .pad = 0},
// meaning even if we do not use it we need a pin for now
		[7] = {.port = GPIOD, .pad = 1},
	},
	.en = {.port = GPIOA, .pad = 6} // copy-paste with setMode code!
};

    if (engineConfiguration->engineType == engine_type_e::FRANKENSO_TEST_33810) {
	    int ret = mc33810_add(Gpio::MC33810_0_OUT_0, 0, &mc33810);
	    efiPrintf("*****************+ mc33810_add %d +*******************", ret);

#ifndef EFI_BOOTLOADER
	    // todo: add to more appropriate location?
	    addConsoleAction("injinfo", [](){
	        efiPrintf("injinfo index=%d", engine->fuelComputer.brokenInjector);
	    });
#endif // EFI_BOOTLOADER
	}
}
