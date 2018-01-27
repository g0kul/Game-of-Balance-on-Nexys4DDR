//Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2017.2 (lin64) Build 1909853 Thu Jun 15 18:39:10 MDT 2017
//Date        : Sat Nov 25 19:08:49 2017
//Host        : dots running 64-bit Ubuntu 16.04.3 LTS
//Command     : generate_target system_wrapper.bd
//Design      : system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module system_wrapper
   (ACL_CSN,
    ACL_MISO,
    ACL_MOSI,
    ACL_SCLK,
    DDR2_addr,
    DDR2_ba,
    DDR2_cas_n,
    DDR2_ck_n,
    DDR2_ck_p,
    DDR2_cke,
    DDR2_cs_n,
    DDR2_dm,
    DDR2_dq,
    DDR2_dqs_n,
    DDR2_dqs_p,
    DDR2_odt,
    DDR2_ras_n,
    DDR2_we_n,
    btn,
    btnCpuReset,
    clock_rtl,
    io0_o,
    led,
    push,
    reset_spi,
    sck_o,
    spi_dc,
    ss_o,
    twist);
  output [0:0]ACL_CSN;
  input ACL_MISO;
  output ACL_MOSI;
  output ACL_SCLK;
  output [12:0]DDR2_addr;
  output [2:0]DDR2_ba;
  output DDR2_cas_n;
  output [0:0]DDR2_ck_n;
  output [0:0]DDR2_ck_p;
  output [0:0]DDR2_cke;
  output [0:0]DDR2_cs_n;
  output [1:0]DDR2_dm;
  inout [15:0]DDR2_dq;
  inout [1:0]DDR2_dqs_n;
  inout [1:0]DDR2_dqs_p;
  output [0:0]DDR2_odt;
  output DDR2_ras_n;
  output DDR2_we_n;
  input [4:0]btn;
  input btnCpuReset;
  input clock_rtl;
  output io0_o;
  output [15:0]led;
  input [0:0]push;
  output reset_spi;
  output sck_o;
  output [0:0]spi_dc;
  output [0:0]ss_o;
  input [1:0]twist;

  wire [0:0]ACL_CSN;
  wire ACL_MISO;
  wire ACL_MOSI;
  wire ACL_SCLK;
  wire [12:0]DDR2_addr;
  wire [2:0]DDR2_ba;
  wire DDR2_cas_n;
  wire [0:0]DDR2_ck_n;
  wire [0:0]DDR2_ck_p;
  wire [0:0]DDR2_cke;
  wire [0:0]DDR2_cs_n;
  wire [1:0]DDR2_dm;
  wire [15:0]DDR2_dq;
  wire [1:0]DDR2_dqs_n;
  wire [1:0]DDR2_dqs_p;
  wire [0:0]DDR2_odt;
  wire DDR2_ras_n;
  wire DDR2_we_n;
  wire [4:0]btn;
  wire btnCpuReset;
  wire clock_rtl;
  wire io0_o;
  wire [15:0]led;
  wire [0:0]push;
  wire reset_spi;
  wire sck_o;
  wire [0:0]spi_dc;
  wire [0:0]ss_o;
  wire [1:0]twist;

  system system_i
       (.ACL_CSN(ACL_CSN),
        .ACL_MISO(ACL_MISO),
        .ACL_MOSI(ACL_MOSI),
        .ACL_SCLK(ACL_SCLK),
        .DDR2_addr(DDR2_addr),
        .DDR2_ba(DDR2_ba),
        .DDR2_cas_n(DDR2_cas_n),
        .DDR2_ck_n(DDR2_ck_n),
        .DDR2_ck_p(DDR2_ck_p),
        .DDR2_cke(DDR2_cke),
        .DDR2_cs_n(DDR2_cs_n),
        .DDR2_dm(DDR2_dm),
        .DDR2_dq(DDR2_dq),
        .DDR2_dqs_n(DDR2_dqs_n),
        .DDR2_dqs_p(DDR2_dqs_p),
        .DDR2_odt(DDR2_odt),
        .DDR2_ras_n(DDR2_ras_n),
        .DDR2_we_n(DDR2_we_n),
        .btn(btn),
        .btnCpuReset(btnCpuReset),
        .clock_rtl(clock_rtl),
        .io0_o(io0_o),
        .led(led),
        .push(push),
        .reset_spi(reset_spi),
        .sck_o(sck_o),
        .spi_dc(spi_dc),
        .ss_o(ss_o),
        .twist(twist));
endmodule
