MAKETOPDIR = ../..

TARGET = libborg_hw.a

SRC_SIM :=

include $(MAKETOPDIR)/defaults.mk


ifeq ($(BORG_HW),HW_BORG_16)
  SRC = borg_hw_borg16.c
endif

ifeq ($(BORG_HW),HW_BORG_16_HGM)
  SRC = borg_hw_borg16_hgmod.c
endif

ifeq ($(BORG_HW),HW_BORG_ANDRE)
  SRC = borg_hw_andreborg.c
endif

ifeq ($(BORG_HW),HW_BORG_LS)
  SRC = borg_hw_borg_ls.c
endif

ifeq ($(BORG_HW),HW_BORG_MH)
  SRC = borg_hw_borg_mh.c
endif

ifeq ($(BORG_HW),HW_LOLSHIELD)
  SRC = borg_hw_lolshield.c
endif

ifeq ($(BORG_HW),HW_BORG_LSJO)
  SRC = borg_hw_borg_lsjo.c
endif

ifeq ($(BORG_HW),HW_LEDBRETT)
  SRC = borg_hw_ledbrett.c
endif

ifeq ($(BORG_HW),HW_BORG_MINI)
  SRC = borg_hw_borg_mini.c
endif

ifeq ($(BORG_HW),HW_PANEL_ONE)
  SRC = borg_hw_panel_one.c
endif

ifeq ($(BORG_HW),HW_PD1165)
  SRC = borg_hw_pd1165.c
endif

ifeq ($(BORG_HW),HW_PINGPONG)
  SRC = borg_hw_pingpong.c
endif

ifeq ($(BORG_HW),HW_ROTOR)
  SRC = borg_hw_rotor.c
endif

ifeq ($(BORG_HW),HW_GIGABORG)
  SRC = borg_hw_gigaborg.c
endif

ifeq ($(BORG_HW),HW_ANCIENTBORG)
  SRC = borg_hw_ancient.c
endif

ifeq ($(BORG_HW),HW_LED_TESTER)
  SRC = borg_hw_led_tester.c
endif

ifeq ($(SRC),'')
  $(error no valid hardware driver selected )
endif

include $(MAKETOPDIR)/rules.mk

include $(MAKETOPDIR)/depend.mk
