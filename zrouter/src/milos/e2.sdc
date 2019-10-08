create_clock -name CLK30 -period 32 -waveform { 0 16 }  clk30m72

create_clock -name CLK122 -period 8 -waveform { 0 4 }   clk122m88

create_clock -name RxClk -period 8 -waveform { 0 4 }   eth_rmii_clk

set_case_analysis 0  TESTACT_CSPMODE

set_propagated_clock [all_clocks]

set_false_path -to EMAC_MACRO/eth_inst/dma_inst/txdma_inst/LOCKUP
set_false_path -to EMAC_MACRO/eth_inst/mac110_inst/mim_inst/LOCKUP
set_false_path -to LOCKUP1
set_false_path -to EMAC_MACRO/eth_inst/dma_inst/txdma_inst/dmatxf_inst/tmatxf_mem_inst/LOCKUP
set_false_path -to EMAC_MACRO/eth_inst/mac110_inst/mcs_inst/LOCKUP
set_false_path -to EMAC_MACRO/eth_inst/dma_inst/rxdma_inst/LOCKUP
set_false_path -to LOCKUP

set timing_latch_required_on_open false
