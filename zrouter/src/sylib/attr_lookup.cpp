#include "ads.h"
BEGIN_NAMESPACE_ADS

/* This code is from Synopsys open-source liberty_parse-1.8  */
/* Added the 3 lines below.    wfs 8/20/04                   */
#ifdef __GNUC__
#undef __GNUC__
#endif

/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -L ANSI-C -t -H attr_hash_func -N lookup_attr_name -C -I -k '*' -D attr_lookup  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "attr_lookup"

#include "sy_attr_enum.h"
#line 4 "attr_lookup"
struct libGroupMap { char *name; sy_attr_enum type; };
#include <string.h>

#define TOTAL_KEYWORDS 500
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 38
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 3069
/* maximum key range = 3066, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
attr_hash_func (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,    0,   65,
        25,   70,   40,   20,   45,   30, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070,    5,  490,    0,  667,    5,
        90,    0,    5,  415,  433,    0,  265,   95,    0,  215,
         5,   10,    0,   55,    0,    0,   30,  135,  130,  378,
       290,  677,   15, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070, 3070,
      3070, 3070, 3070, 3070, 3070, 3070, 3070
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[37]];
      /*FALLTHROUGH*/
      case 37:
        hval += asso_values[(unsigned char)str[36]];
      /*FALLTHROUGH*/
      case 36:
        hval += asso_values[(unsigned char)str[35]];
      /*FALLTHROUGH*/
      case 35:
        hval += asso_values[(unsigned char)str[34]];
      /*FALLTHROUGH*/
      case 34:
        hval += asso_values[(unsigned char)str[33]];
      /*FALLTHROUGH*/
      case 33:
        hval += asso_values[(unsigned char)str[32]];
      /*FALLTHROUGH*/
      case 32:
        hval += asso_values[(unsigned char)str[31]];
      /*FALLTHROUGH*/
      case 31:
        hval += asso_values[(unsigned char)str[30]];
      /*FALLTHROUGH*/
      case 30:
        hval += asso_values[(unsigned char)str[29]];
      /*FALLTHROUGH*/
      case 29:
        hval += asso_values[(unsigned char)str[28]];
      /*FALLTHROUGH*/
      case 28:
        hval += asso_values[(unsigned char)str[27]];
      /*FALLTHROUGH*/
      case 27:
        hval += asso_values[(unsigned char)str[26]];
      /*FALLTHROUGH*/
      case 26:
        hval += asso_values[(unsigned char)str[25]];
      /*FALLTHROUGH*/
      case 25:
        hval += asso_values[(unsigned char)str[24]];
      /*FALLTHROUGH*/
      case 24:
        hval += asso_values[(unsigned char)str[23]];
      /*FALLTHROUGH*/
      case 23:
        hval += asso_values[(unsigned char)str[22]];
      /*FALLTHROUGH*/
      case 22:
        hval += asso_values[(unsigned char)str[21]];
      /*FALLTHROUGH*/
      case 21:
        hval += asso_values[(unsigned char)str[20]];
      /*FALLTHROUGH*/
      case 20:
        hval += asso_values[(unsigned char)str[19]];
      /*FALLTHROUGH*/
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
      case 18:
        hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
      case 17:
        hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
      case 16:
        hval += asso_values[(unsigned char)str[15]];
      /*FALLTHROUGH*/
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]+1];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
const struct libGroupMap *
lookup_attr_name (register const char *str, register unsigned int len)
{
  static const struct libGroupMap wordlist[] =
    {
#line 349 "attr_lookup"
      {"area",	LIBERTY_ATTRENUM_area},
#line 194 "attr_lookup"
      {"clear",	LIBERTY_ATTRENUM_clear},
#line 128 "attr_lookup"
      {"slope",	LIBERTY_ATTRENUM_slope},
#line 142 "attr_lookup"
      {"process",	LIBERTY_ATTRENUM_process},
#line 241 "attr_lookup"
      {"coefs",	LIBERTY_ATTRENUM_coefs},
#line 144 "attr_lookup"
      {"cell_name",	LIBERTY_ATTRENUM_cell_name},
#line 189 "attr_lookup"
      {"preset",	LIBERTY_ATTRENUM_preset},
#line 129 "attr_lookup"
      {"resistance",	LIBERTY_ATTRENUM_resistance},
#line 168 "attr_lookup"
      {"force_01",	LIBERTY_ATTRENUM_force_01},
#line 312 "attr_lookup"
      {"pin_opposite",	LIBERTY_ATTRENUM_pin_opposite},
#line 73 "attr_lookup"
      {"piece_type",	LIBERTY_ATTRENUM_piece_type},
#line 12 "attr_lookup"
      {"rise_pin_resistance",	LIBERTY_ATTRENUM_rise_pin_resistance},
#line 14 "attr_lookup"
      {"fall_pin_resistance",	LIBERTY_ATTRENUM_fall_pin_resistance},
#line 214 "attr_lookup"
      {"rise_resistance",	LIBERTY_ATTRENUM_rise_resistance},
#line 36 "attr_lookup"
      {"area_coefficient",	LIBERTY_ATTRENUM_area_coefficient},
#line 220 "attr_lookup"
      {"fall_resistance",	LIBERTY_ATTRENUM_fall_resistance},
#line 169 "attr_lookup"
      {"force_00",	LIBERTY_ATTRENUM_force_00},
#line 311 "attr_lookup"
      {"rail_connection",	LIBERTY_ATTRENUM_rail_connection},
#line 282 "attr_lookup"
      {"is_pad",	LIBERTY_ATTRENUM_is_pad},
#line 32 "attr_lookup"
      {"lsi_pad",	LIBERTY_ATTRENUM_lsi_pad},
#line 347 "attr_lookup"
      {"cell_footprint",	LIBERTY_ATTRENUM_cell_footprint},
#line 242 "attr_lookup"
      {"orders",	LIBERTY_ATTRENUM_orders},
#line 322 "attr_lookup"
      {"preferred",	LIBERTY_ATTRENUM_preferred},
#line 309 "attr_lookup"
      {"clock",	LIBERTY_ATTRENUM_clock},
#line 166 "attr_lookup"
      {"force_11",	LIBERTY_ATTRENUM_force_11},
#line 502 "attr_lookup"
      {"date",	LIBERTY_ATTRENUM_date},
#line 262 "attr_lookup"
      {"rise_capacitance",	LIBERTY_ATTRENUM_rise_capacitance},
#line 70 "attr_lookup"
      {"piece_define",	LIBERTY_ATTRENUM_piece_define},
#line 224 "attr_lookup"
      {"sdf_cond",	LIBERTY_ATTRENUM_sdf_cond},
#line 297 "attr_lookup"
      {"fall_capacitance",	LIBERTY_ATTRENUM_fall_capacitance},
#line 147 "attr_lookup"
      {"vil",	LIBERTY_ATTRENUM_vil},
#line 170 "attr_lookup"
      {"tracks",	LIBERTY_ATTRENUM_tracks},
#line 180 "attr_lookup"
      {"data_in",	LIBERTY_ATTRENUM_data_in},
#line 300 "attr_lookup"
      {"direction",	LIBERTY_ATTRENUM_direction},
#line 136 "attr_lookup"
      {"vol",	LIBERTY_ATTRENUM_vol},
#line 319 "attr_lookup"
      {"set_node",	LIBERTY_ATTRENUM_set_node},
#line 364 "attr_lookup"
      {"revision",	LIBERTY_ATTRENUM_revision},
#line 167 "attr_lookup"
      {"force_10",	LIBERTY_ATTRENUM_force_10},
#line 185 "attr_lookup"
      {"invert",	LIBERTY_ATTRENUM_invert},
#line 42 "attr_lookup"
      {"reference_capacitance",	LIBERTY_ATTRENUM_reference_capacitance},
#line 109 "attr_lookup"
      {"k_process_cell_rise",	LIBERTY_ATTRENUM_k_process_cell_rise},
#line 448 "attr_lookup"
      {"k_process_pin_cap",	LIBERTY_ATTRENUM_k_process_pin_cap},
#line 110 "attr_lookup"
      {"k_process_cell_fall",	LIBERTY_ATTRENUM_k_process_cell_fall},
#line 436 "attr_lookup"
      {"k_process_slope_rise",	LIBERTY_ATTRENUM_k_process_slope_rise},
#line 229 "attr_lookup"
      {"address",	LIBERTY_ATTRENUM_address},
#line 437 "attr_lookup"
      {"k_process_slope_fall",	LIBERTY_ATTRENUM_k_process_slope_fall},
#line 446 "attr_lookup"
      {"k_process_pin_rise_cap",	LIBERTY_ATTRENUM_k_process_pin_rise_cap},
#line 447 "attr_lookup"
      {"k_process_pin_fall_cap",	LIBERTY_ATTRENUM_k_process_pin_fall_cap},
#line 212 "attr_lookup"
      {"sdf_cond_start",	LIBERTY_ATTRENUM_sdf_cond_start},
#line 289 "attr_lookup"
      {"function",	LIBERTY_ATTRENUM_function},
#line 51 "attr_lookup"
      {"isolation_cell_data_pin",	LIBERTY_ATTRENUM_isolation_cell_data_pin},
#line 353 "attr_lookup"
      {"define_cell_area",	LIBERTY_ATTRENUM_define_cell_area},
#line 192 "attr_lookup"
      {"clear_preset_var2",	LIBERTY_ATTRENUM_clear_preset_var2},
#line 454 "attr_lookup"
      {"k_process_intrinsic_rise",	LIBERTY_ATTRENUM_k_process_intrinsic_rise},
#line 171 "attr_lookup"
      {"total_track_area",	LIBERTY_ATTRENUM_total_track_area},
#line 455 "attr_lookup"
      {"k_process_intrinsic_fall",	LIBERTY_ATTRENUM_k_process_intrinsic_fall},
#line 213 "attr_lookup"
      {"sdf_cond_end",	LIBERTY_ATTRENUM_sdf_cond_end},
#line 103 "attr_lookup"
      {"k_process_rise_pin_resistance",	LIBERTY_ATTRENUM_k_process_rise_pin_resistance},
#line 71 "attr_lookup"
      {"resistance_unit",	LIBERTY_ATTRENUM_resistance_unit},
#line 107 "attr_lookup"
      {"k_process_fall_pin_resistance",	LIBERTY_ATTRENUM_k_process_fall_pin_resistance},
#line 21 "attr_lookup"
      {"setup_coefficient",	LIBERTY_ATTRENUM_setup_coefficient},
#line 266 "attr_lookup"
      {"prefer_tied",	LIBERTY_ATTRENUM_prefer_tied},
#line 123 "attr_lookup"
      {"fanout_resistance",	LIBERTY_ATTRENUM_fanout_resistance},
#line 37 "attr_lookup"
      {"calc_mode",	LIBERTY_ATTRENUM_calc_mode},
#line 193 "attr_lookup"
      {"clear_preset_var1",	LIBERTY_ATTRENUM_clear_preset_var1},
#line 328 "attr_lookup"
      {"map_only",	LIBERTY_ATTRENUM_map_only},
#line 101 "attr_lookup"
      {"k_process_rise_transition",	LIBERTY_ATTRENUM_k_process_rise_transition},
#line 105 "attr_lookup"
      {"k_process_fall_transition",	LIBERTY_ATTRENUM_k_process_fall_transition},
#line 255 "attr_lookup"
      {"state_function",	LIBERTY_ATTRENUM_state_function},
#line 178 "attr_lookup"
      {"value",	LIBERTY_ATTRENUM_value},
#line 243 "attr_lookup"
      {"values",	LIBERTY_ATTRENUM_values},
#line 6 "attr_lookup"
      {"parameter5",	LIBERTY_ATTRENUM_parameter5},
#line 9 "attr_lookup"
      {"parameter2",	LIBERTY_ATTRENUM_parameter2},
#line 341 "attr_lookup"
      {"dont_use",	LIBERTY_ATTRENUM_dont_use},
#line 209 "attr_lookup"
      {"slope_rise",	LIBERTY_ATTRENUM_slope_rise},
#line 7 "attr_lookup"
      {"parameter4",	LIBERTY_ATTRENUM_parameter4},
#line 343 "attr_lookup"
      {"dont_fault",	LIBERTY_ATTRENUM_dont_fault},
#line 198 "attr_lookup"
      {"mode",	LIBERTY_ATTRENUM_mode},
#line 184 "attr_lookup"
      {"master_pin",	LIBERTY_ATTRENUM_master_pin},
#line 324 "attr_lookup"
      {"pad_cell",	LIBERTY_ATTRENUM_pad_cell},
#line 272 "attr_lookup"
      {"min_transition",	LIBERTY_ATTRENUM_min_transition},
#line 10 "attr_lookup"
      {"parameter1",	LIBERTY_ATTRENUM_parameter1},
#line 49 "attr_lookup"
      {"level_shifter_data_pin",	LIBERTY_ATTRENUM_level_shifter_data_pin},
#line 217 "attr_lookup"
      {"intrinsic_rise",	LIBERTY_ATTRENUM_intrinsic_rise},
#line 8 "attr_lookup"
      {"parameter3",	LIBERTY_ATTRENUM_parameter3},
#line 218 "attr_lookup"
      {"intrinsic_fall",	LIBERTY_ATTRENUM_intrinsic_fall},
#line 382 "attr_lookup"
      {"k_volt_slope_rise",	LIBERTY_ATTRENUM_k_volt_slope_rise},
#line 440 "attr_lookup"
      {"k_process_setup_rise",	LIBERTY_ATTRENUM_k_process_setup_rise},
#line 383 "attr_lookup"
      {"k_volt_slope_fall",	LIBERTY_ATTRENUM_k_volt_slope_fall},
#line 124 "attr_lookup"
      {"fanout_capacitance",	LIBERTY_ATTRENUM_fanout_capacitance},
#line 441 "attr_lookup"
      {"k_process_setup_fall",	LIBERTY_ATTRENUM_k_process_setup_fall},
#line 373 "attr_lookup"
      {"nom_process",	LIBERTY_ATTRENUM_nom_process},
#line 145 "attr_lookup"
      {"vimin",	LIBERTY_ATTRENUM_vimin},
#line 81 "attr_lookup"
      {"k_volt_rise_pin_resistance",	LIBERTY_ATTRENUM_k_volt_rise_pin_resistance},
#line 392 "attr_lookup"
      {"k_volt_pin_cap",	LIBERTY_ATTRENUM_k_volt_pin_cap},
#line 223 "attr_lookup"
      {"constraint",	LIBERTY_ATTRENUM_constraint},
#line 11 "attr_lookup"
      {"reference_time",	LIBERTY_ATTRENUM_reference_time},
#line 134 "attr_lookup"
      {"vomin",	LIBERTY_ATTRENUM_vomin},
#line 302 "attr_lookup"
      {"connection_class",	LIBERTY_ATTRENUM_connection_class},
#line 62 "attr_lookup"
      {"user_function_class",	LIBERTY_ATTRENUM_user_function_class},
#line 284 "attr_lookup"
      {"internal_node",	LIBERTY_ATTRENUM_internal_node},
#line 79 "attr_lookup"
      {"k_volt_rise_transition",	LIBERTY_ATTRENUM_k_volt_rise_transition},
#line 87 "attr_lookup"
      {"k_volt_cell_rise",	LIBERTY_ATTRENUM_k_volt_cell_rise},
#line 88 "attr_lookup"
      {"k_volt_cell_fall",	LIBERTY_ATTRENUM_k_volt_cell_fall},
#line 459 "attr_lookup"
      {"k_process_drive_rise",	LIBERTY_ATTRENUM_k_process_drive_rise},
#line 401 "attr_lookup"
      {"k_volt_hold_rise",	LIBERTY_ATTRENUM_k_volt_hold_rise},
#line 188 "attr_lookup"
      {"clock_pin",	LIBERTY_ATTRENUM_clock_pin},
#line 460 "attr_lookup"
      {"k_process_drive_fall",	LIBERTY_ATTRENUM_k_process_drive_fall},
#line 402 "attr_lookup"
      {"k_volt_hold_fall",	LIBERTY_ATTRENUM_k_volt_hold_fall},
#line 397 "attr_lookup"
      {"k_volt_min_period",	LIBERTY_ATTRENUM_k_volt_min_period},
#line 299 "attr_lookup"
      {"drive_current",	LIBERTY_ATTRENUM_drive_current},
#line 276 "attr_lookup"
      {"min_fanout",	LIBERTY_ATTRENUM_min_fanout},
#line 409 "attr_lookup"
      {"k_temp_slope_rise",	LIBERTY_ATTRENUM_k_temp_slope_rise},
#line 410 "attr_lookup"
      {"k_temp_slope_fall",	LIBERTY_ATTRENUM_k_temp_slope_fall},
#line 246 "attr_lookup"
      {"index_2",	LIBERTY_ATTRENUM_index_2},
#line 403 "attr_lookup"
      {"k_volt_drive_rise",	LIBERTY_ATTRENUM_k_volt_drive_rise},
#line 92 "attr_lookup"
      {"k_temp_rise_pin_resistance",	LIBERTY_ATTRENUM_k_temp_rise_pin_resistance},
#line 404 "attr_lookup"
      {"k_volt_drive_fall",	LIBERTY_ATTRENUM_k_volt_drive_fall},
#line 419 "attr_lookup"
      {"k_temp_pin_cap",	LIBERTY_ATTRENUM_k_temp_pin_cap},
#line 177 "attr_lookup"
      {"input_pins",	LIBERTY_ATTRENUM_input_pins},
#line 90 "attr_lookup"
      {"k_temp_rise_transition",	LIBERTY_ATTRENUM_k_temp_rise_transition},
#line 256 "attr_lookup"
      {"slew_control",	LIBERTY_ATTRENUM_slew_control},
#line 98 "attr_lookup"
      {"k_temp_cell_rise",	LIBERTY_ATTRENUM_k_temp_cell_rise},
#line 247 "attr_lookup"
      {"index_1",	LIBERTY_ATTRENUM_index_1},
#line 99 "attr_lookup"
      {"k_temp_cell_fall",	LIBERTY_ATTRENUM_k_temp_cell_fall},
#line 245 "attr_lookup"
      {"index_3",	LIBERTY_ATTRENUM_index_3},
#line 428 "attr_lookup"
      {"k_temp_hold_rise",	LIBERTY_ATTRENUM_k_temp_hold_rise},
#line 181 "attr_lookup"
      {"shifts",	LIBERTY_ATTRENUM_shifts},
#line 429 "attr_lookup"
      {"k_temp_hold_fall",	LIBERTY_ATTRENUM_k_temp_hold_fall},
#line 143 "attr_lookup"
      {"short",	LIBERTY_ATTRENUM_short},
#line 424 "attr_lookup"
      {"k_temp_min_period",	LIBERTY_ATTRENUM_k_temp_min_period},
#line 504 "attr_lookup"
      {"comment",	LIBERTY_ATTRENUM_comment},
#line 291 "attr_lookup"
      {"fault_model",	LIBERTY_ATTRENUM_fault_model},
#line 386 "attr_lookup"
      {"k_volt_setup_rise",	LIBERTY_ATTRENUM_k_volt_setup_rise},
#line 453 "attr_lookup"
      {"k_process_min_period",	LIBERTY_ATTRENUM_k_process_min_period},
#line 387 "attr_lookup"
      {"k_volt_setup_fall",	LIBERTY_ATTRENUM_k_volt_setup_fall},
#line 430 "attr_lookup"
      {"k_temp_drive_rise",	LIBERTY_ATTRENUM_k_temp_drive_rise},
#line 222 "attr_lookup"
      {"fsim_map",	LIBERTY_ATTRENUM_fsim_map},
#line 431 "attr_lookup"
      {"k_temp_drive_fall",	LIBERTY_ATTRENUM_k_temp_drive_fall},
#line 182 "attr_lookup"
      {"edges",	LIBERTY_ATTRENUM_edges},
#line 131 "attr_lookup"
      {"faster_factor",	LIBERTY_ATTRENUM_faster_factor},
#line 292 "attr_lookup"
      {"fanout_load",	LIBERTY_ATTRENUM_fanout_load},
#line 283 "attr_lookup"
      {"inverted_output",	LIBERTY_ATTRENUM_inverted_output},
#line 141 "attr_lookup"
      {"temperature",	LIBERTY_ATTRENUM_temperature},
#line 159 "attr_lookup"
      {"downto",	LIBERTY_ATTRENUM_downto},
#line 442 "attr_lookup"
      {"k_process_removal_rise",	LIBERTY_ATTRENUM_k_process_removal_rise},
#line 443 "attr_lookup"
      {"k_process_removal_fall",	LIBERTY_ATTRENUM_k_process_removal_fall},
#line 33 "attr_lookup"
      {"power_level",	LIBERTY_ATTRENUM_power_level},
#line 344 "attr_lookup"
      {"contention_condition",	LIBERTY_ATTRENUM_contention_condition},
#line 434 "attr_lookup"
      {"k_process_wire_res",	LIBERTY_ATTRENUM_k_process_wire_res},
#line 52 "attr_lookup"
      {"is_three_state",	LIBERTY_ATTRENUM_is_three_state},
#line 435 "attr_lookup"
      {"k_process_wire_cap",	LIBERTY_ATTRENUM_k_process_wire_cap},
#line 30 "attr_lookup"
      {"rise_capacitance_range",	LIBERTY_ATTRENUM_rise_capacitance_range},
#line 31 "attr_lookup"
      {"fall_capacitance_range",	LIBERTY_ATTRENUM_fall_capacitance_range},
#line 16 "attr_lookup"
      {"tied_off",	LIBERTY_ATTRENUM_tied_off},
#line 148 "attr_lookup"
      {"vih",	LIBERTY_ATTRENUM_vih},
#line 413 "attr_lookup"
      {"k_temp_setup_rise",	LIBERTY_ATTRENUM_k_temp_setup_rise},
#line 244 "attr_lookup"
      {"intermediate_values",	LIBERTY_ATTRENUM_intermediate_values},
#line 414 "attr_lookup"
      {"k_temp_setup_fall",	LIBERTY_ATTRENUM_k_temp_setup_fall},
#line 461 "attr_lookup"
      {"k_process_drive_current",	LIBERTY_ATTRENUM_k_process_drive_current},
#line 320 "attr_lookup"
      {"scan_group",	LIBERTY_ATTRENUM_scan_group},
#line 137 "attr_lookup"
      {"voh",	LIBERTY_ATTRENUM_voh},
#line 468 "attr_lookup"
      {"force_inverter_removal",	LIBERTY_ATTRENUM_force_inverter_removal},
#line 174 "attr_lookup"
      {"row_address",	LIBERTY_ATTRENUM_row_address},
#line 380 "attr_lookup"
      {"k_volt_wire_res",	LIBERTY_ATTRENUM_k_volt_wire_res},
#line 381 "attr_lookup"
      {"k_volt_wire_cap",	LIBERTY_ATTRENUM_k_volt_wire_cap},
#line 139 "attr_lookup"
      {"voltage",	LIBERTY_ATTRENUM_voltage},
#line 77 "attr_lookup"
      {"lsi_pad_rise",	LIBERTY_ATTRENUM_lsi_pad_rise},
#line 275 "attr_lookup"
      {"min_period",	LIBERTY_ATTRENUM_min_period},
#line 78 "attr_lookup"
      {"lsi_pad_fall",	LIBERTY_ATTRENUM_lsi_pad_fall},
#line 405 "attr_lookup"
      {"k_volt_drive_current",	LIBERTY_ATTRENUM_k_volt_drive_current},
#line 456 "attr_lookup"
      {"k_process_internal_power",	LIBERTY_ATTRENUM_k_process_internal_power},
#line 278 "attr_lookup"
      {"max_transition",	LIBERTY_ATTRENUM_max_transition},
#line 398 "attr_lookup"
      {"k_volt_intrinsic_rise",	LIBERTY_ATTRENUM_k_volt_intrinsic_rise},
#line 399 "attr_lookup"
      {"k_volt_intrinsic_fall",	LIBERTY_ATTRENUM_k_volt_intrinsic_fall},
#line 211 "attr_lookup"
      {"sdf_edges",	LIBERTY_ATTRENUM_sdf_edges},
#line 24 "attr_lookup"
      {"hold_coefficient",	LIBERTY_ATTRENUM_hold_coefficient},
#line 248 "attr_lookup"
      {"related_pin",	LIBERTY_ATTRENUM_related_pin},
#line 240 "attr_lookup"
      {"equal_or_opposite_output",	LIBERTY_ATTRENUM_equal_or_opposite_output},
#line 146 "attr_lookup"
      {"vimax",	LIBERTY_ATTRENUM_vimax},
#line 151 "attr_lookup"
      {"mapping",	LIBERTY_ATTRENUM_mapping},
#line 253 "attr_lookup"
      {"three_state",	LIBERTY_ATTRENUM_three_state},
#line 438 "attr_lookup"
      {"k_process_skew_rise",	LIBERTY_ATTRENUM_k_process_skew_rise},
#line 135 "attr_lookup"
      {"vomax",	LIBERTY_ATTRENUM_vomax},
#line 102 "attr_lookup"
      {"k_process_rise_propagation",	LIBERTY_ATTRENUM_k_process_rise_propagation},
#line 439 "attr_lookup"
      {"k_process_skew_fall",	LIBERTY_ATTRENUM_k_process_skew_fall},
#line 388 "attr_lookup"
      {"k_volt_removal_rise",	LIBERTY_ATTRENUM_k_volt_removal_rise},
#line 106 "attr_lookup"
      {"k_process_fall_propagation",	LIBERTY_ATTRENUM_k_process_fall_propagation},
#line 389 "attr_lookup"
      {"k_volt_removal_fall",	LIBERTY_ATTRENUM_k_volt_removal_fall},
#line 407 "attr_lookup"
      {"k_temp_wire_res",	LIBERTY_ATTRENUM_k_temp_wire_res},
#line 74 "attr_lookup"
      {"nom_calc_mode",	LIBERTY_ATTRENUM_nom_calc_mode},
#line 54 "attr_lookup"
      {"edge_rate_rise",	LIBERTY_ATTRENUM_edge_rate_rise},
#line 408 "attr_lookup"
      {"k_temp_wire_cap",	LIBERTY_ATTRENUM_k_temp_wire_cap},
#line 326 "attr_lookup"
      {"mpm_name",	LIBERTY_ATTRENUM_mpm_name},
#line 57 "attr_lookup"
      {"edge_rate_fall",	LIBERTY_ATTRENUM_edge_rate_fall},
#line 363 "attr_lookup"
      {"simulation",	LIBERTY_ATTRENUM_simulation},
#line 69 "attr_lookup"
      {"failure_rate",	LIBERTY_ATTRENUM_failure_rate},
#line 316 "attr_lookup"
      {"vhdl_name",	LIBERTY_ATTRENUM_vhdl_name},
#line 227 "attr_lookup"
      {"enable",	LIBERTY_ATTRENUM_enable},
#line 432 "attr_lookup"
      {"k_temp_drive_current",	LIBERTY_ATTRENUM_k_temp_drive_current},
#line 425 "attr_lookup"
      {"k_temp_intrinsic_rise",	LIBERTY_ATTRENUM_k_temp_intrinsic_rise},
#line 426 "attr_lookup"
      {"k_temp_intrinsic_fall",	LIBERTY_ATTRENUM_k_temp_intrinsic_fall},
#line 357 "attr_lookup"
      {"time_unit",	LIBERTY_ATTRENUM_time_unit},
#line 280 "attr_lookup"
      {"max_fanout",	LIBERTY_ATTRENUM_max_fanout},
#line 210 "attr_lookup"
      {"slope_fall",	LIBERTY_ATTRENUM_slope_fall},
#line 165 "attr_lookup"
      {"table",	LIBERTY_ATTRENUM_table},
#line 457 "attr_lookup"
      {"k_process_hold_rise",	LIBERTY_ATTRENUM_k_process_hold_rise},
#line 72 "attr_lookup"
      {"power_model",	LIBERTY_ATTRENUM_power_model},
#line 173 "attr_lookup"
      {"type",	LIBERTY_ATTRENUM_type},
#line 458 "attr_lookup"
      {"k_process_hold_fall",	LIBERTY_ATTRENUM_k_process_hold_fall},
#line 372 "attr_lookup"
      {"nom_temperature",	LIBERTY_ATTRENUM_nom_temperature},
#line 202 "attr_lookup"
      {"timing_sense",	LIBERTY_ATTRENUM_timing_sense},
#line 228 "attr_lookup"
      {"clocked_on",	LIBERTY_ATTRENUM_clocked_on},
#line 271 "attr_lookup"
      {"multicell_pad_pin",	LIBERTY_ATTRENUM_multicell_pad_pin},
#line 356 "attr_lookup"
      {"timing_report",	LIBERTY_ATTRENUM_timing_report},
#line 310 "attr_lookup"
      {"capacitance",	LIBERTY_ATTRENUM_capacitance},
#line 415 "attr_lookup"
      {"k_temp_removal_rise",	LIBERTY_ATTRENUM_k_temp_removal_rise},
#line 163 "attr_lookup"
      {"bit_from",	LIBERTY_ATTRENUM_bit_from},
#line 416 "attr_lookup"
      {"k_temp_removal_fall",	LIBERTY_ATTRENUM_k_temp_removal_fall},
#line 354 "attr_lookup"
      {"capacitive_load_unit",	LIBERTY_ATTRENUM_capacitive_load_unit},
#line 332 "attr_lookup"
      {"geometry_print",	LIBERTY_ATTRENUM_geometry_print},
#line 191 "attr_lookup"
      {"clocked_on_also",	LIBERTY_ATTRENUM_clocked_on_also},
#line 465 "attr_lookup"
      {"in_place_swap_mode",	LIBERTY_ATTRENUM_in_place_swap_mode},
#line 162 "attr_lookup"
      {"bit_to",	LIBERTY_ATTRENUM_bit_to},
#line 67 "attr_lookup"
      {"is_isolation_cell",	LIBERTY_ATTRENUM_is_isolation_cell},
#line 225 "attr_lookup"
      {"constraint_low",	LIBERTY_ATTRENUM_constraint_low},
#line 342 "attr_lookup"
      {"dont_touch",	LIBERTY_ATTRENUM_dont_touch},
#line 285 "attr_lookup"
      {"input_voltage",	LIBERTY_ATTRENUM_input_voltage},
#line 252 "attr_lookup"
      {"x_function",	LIBERTY_ATTRENUM_x_function},
#line 50 "attr_lookup"
      {"isolation_cell_enable_pin",	LIBERTY_ATTRENUM_isolation_cell_enable_pin},
#line 85 "attr_lookup"
      {"k_volt_fall_pin_resistance",	LIBERTY_ATTRENUM_k_volt_fall_pin_resistance},
#line 80 "attr_lookup"
      {"k_volt_rise_propagation",	LIBERTY_ATTRENUM_k_volt_rise_propagation},
#line 55 "attr_lookup"
      {"edge_rate_load_rise",	LIBERTY_ATTRENUM_edge_rate_load_rise},
#line 56 "attr_lookup"
      {"edge_rate_load_fall",	LIBERTY_ATTRENUM_edge_rate_load_fall},
#line 477 "attr_lookup"
      {"default_slope_rise",	LIBERTY_ATTRENUM_default_slope_rise},
#line 376 "attr_lookup"
      {"key_seed",	LIBERTY_ATTRENUM_key_seed},
#line 478 "attr_lookup"
      {"default_slope_fall",	LIBERTY_ATTRENUM_default_slope_fall},
#line 237 "attr_lookup"
      {"related_input",	LIBERTY_ATTRENUM_related_input},
#line 236 "attr_lookup"
      {"related_inputs",	LIBERTY_ATTRENUM_related_inputs},
#line 377 "attr_lookup"
      {"key_file",	LIBERTY_ATTRENUM_key_file},
#line 83 "attr_lookup"
      {"k_volt_fall_transition",	LIBERTY_ATTRENUM_k_volt_fall_transition},
#line 384 "attr_lookup"
      {"k_volt_skew_rise",	LIBERTY_ATTRENUM_k_volt_skew_rise},
#line 196 "attr_lookup"
      {"tdisable",	LIBERTY_ATTRENUM_tdisable},
#line 385 "attr_lookup"
      {"k_volt_skew_fall",	LIBERTY_ATTRENUM_k_volt_skew_fall},
#line 140 "attr_lookup"
      {"tree_type",	LIBERTY_ATTRENUM_tree_type},
#line 463 "attr_lookup"
      {"input_threshold_pct_rise",	LIBERTY_ATTRENUM_input_threshold_pct_rise},
#line 158 "attr_lookup"
      {"variables",	LIBERTY_ATTRENUM_variables},
#line 464 "attr_lookup"
      {"input_threshold_pct_fall",	LIBERTY_ATTRENUM_input_threshold_pct_fall},
#line 323 "attr_lookup"
      {"pad_type",	LIBERTY_ATTRENUM_pad_type},
#line 330 "attr_lookup"
      {"interface_timing",	LIBERTY_ATTRENUM_interface_timing},
#line 230 "attr_lookup"
      {"when",	LIBERTY_ATTRENUM_when},
#line 491 "attr_lookup"
      {"default_intrinsic_rise",	LIBERTY_ATTRENUM_default_intrinsic_rise},
#line 111 "attr_lookup"
      {"k_process_cell_degradation",	LIBERTY_ATTRENUM_k_process_cell_degradation},
#line 492 "attr_lookup"
      {"default_intrinsic_fall",	LIBERTY_ATTRENUM_default_intrinsic_fall},
#line 479 "attr_lookup"
      {"default_rise_pin_resistance",	LIBERTY_ATTRENUM_default_rise_pin_resistance},
#line 200 "attr_lookup"
      {"when_end",	LIBERTY_ATTRENUM_when_end},
#line 498 "attr_lookup"
      {"default_fall_pin_resistance",	LIBERTY_ATTRENUM_default_fall_pin_resistance},
#line 66 "attr_lookup"
      {"is_level_shifter",	LIBERTY_ATTRENUM_is_level_shifter},
#line 149 "attr_lookup"
      {"variable_2",	LIBERTY_ATTRENUM_variable_2},
#line 96 "attr_lookup"
      {"k_temp_fall_pin_resistance",	LIBERTY_ATTRENUM_k_temp_fall_pin_resistance},
#line 503 "attr_lookup"
      {"current_unit",	LIBERTY_ATTRENUM_current_unit},
#line 91 "attr_lookup"
      {"k_temp_rise_propagation",	LIBERTY_ATTRENUM_k_temp_rise_propagation},
#line 114 "attr_lookup"
      {"default_reference_capacitance",	LIBERTY_ATTRENUM_default_reference_capacitance},
#line 115 "attr_lookup"
      {"default_rc_rise_coefficient",	LIBERTY_ATTRENUM_default_rc_rise_coefficient},
#line 500 "attr_lookup"
      {"default_connection_class",	LIBERTY_ATTRENUM_default_connection_class},
#line 116 "attr_lookup"
      {"default_rc_fall_coefficient",	LIBERTY_ATTRENUM_default_rc_fall_coefficient},
#line 130 "attr_lookup"
      {"slower_factor",	LIBERTY_ATTRENUM_slower_factor},
#line 125 "attr_lookup"
      {"fanout_area",	LIBERTY_ATTRENUM_fanout_area},
#line 94 "attr_lookup"
      {"k_temp_fall_transition",	LIBERTY_ATTRENUM_k_temp_fall_transition},
#line 411 "attr_lookup"
      {"k_temp_skew_rise",	LIBERTY_ATTRENUM_k_temp_skew_rise},
#line 313 "attr_lookup"
      {"pin_equal",	LIBERTY_ATTRENUM_pin_equal},
#line 412 "attr_lookup"
      {"k_temp_skew_fall",	LIBERTY_ATTRENUM_k_temp_skew_fall},
#line 48 "attr_lookup"
      {"level_shifter_enable_pin",	LIBERTY_ATTRENUM_level_shifter_enable_pin},
#line 150 "attr_lookup"
      {"variable_1",	LIBERTY_ATTRENUM_variable_1},
#line 76 "attr_lookup"
      {"lsi_rounding_cutoff",	LIBERTY_ATTRENUM_lsi_rounding_cutoff},
#line 133 "attr_lookup"
      {"variable_3",	LIBERTY_ATTRENUM_variable_3},
#line 40 "attr_lookup"
      {"poly_convert",	LIBERTY_ATTRENUM_poly_convert},
#line 160 "attr_lookup"
      {"data_type",	LIBERTY_ATTRENUM_data_type},
#line 47 "attr_lookup"
      {"map_to_logic",	LIBERTY_ATTRENUM_map_to_logic},
#line 329 "attr_lookup"
      {"is_clock_gating_cell",	LIBERTY_ATTRENUM_is_clock_gating_cell},
#line 371 "attr_lookup"
      {"nom_voltage",	LIBERTY_ATTRENUM_nom_voltage},
#line 269 "attr_lookup"
      {"output_signal_level",	LIBERTY_ATTRENUM_output_signal_level},
#line 277 "attr_lookup"
      {"min_capacitance",	LIBERTY_ATTRENUM_min_capacitance},
#line 122 "attr_lookup"
      {"default_cell_failure_rate",	LIBERTY_ATTRENUM_default_cell_failure_rate},
#line 493 "attr_lookup"
      {"default_input_pin_cap",	LIBERTY_ATTRENUM_default_input_pin_cap},
#line 375 "attr_lookup"
      {"key_version",	LIBERTY_ATTRENUM_key_version},
#line 235 "attr_lookup"
      {"related_outputs",	LIBERTY_ATTRENUM_related_outputs},
#line 267 "attr_lookup"
      {"pin_func_type",	LIBERTY_ATTRENUM_pin_func_type},
#line 496 "attr_lookup"
      {"default_inout_pin_cap",	LIBERTY_ATTRENUM_default_inout_pin_cap},
#line 494 "attr_lookup"
      {"default_inout_pin_rise_res",	LIBERTY_ATTRENUM_default_inout_pin_rise_res},
#line 314 "attr_lookup"
      {"xnf_schnm",	LIBERTY_ATTRENUM_xnf_schnm},
#line 215 "attr_lookup"
      {"related_output_pin",	LIBERTY_ATTRENUM_related_output_pin},
#line 321 "attr_lookup"
      {"scaling_factors",	LIBERTY_ATTRENUM_scaling_factors},
#line 495 "attr_lookup"
      {"default_inout_pin_fall_res",	LIBERTY_ATTRENUM_default_inout_pin_fall_res},
#line 368 "attr_lookup"
      {"preferred_input_pad_voltage",	LIBERTY_ATTRENUM_preferred_input_pad_voltage},
#line 400 "attr_lookup"
      {"k_volt_internal_power",	LIBERTY_ATTRENUM_k_volt_internal_power},
#line 444 "attr_lookup"
      {"k_process_recovery_rise",	LIBERTY_ATTRENUM_k_process_recovery_rise},
#line 445 "attr_lookup"
      {"k_process_recovery_fall",	LIBERTY_ATTRENUM_k_process_recovery_fall},
#line 34 "attr_lookup"
      {"width_coefficient",	LIBERTY_ATTRENUM_width_coefficient},
#line 113 "attr_lookup"
      {"default_setup_coefficient",	LIBERTY_ATTRENUM_default_setup_coefficient},
#line 286 "attr_lookup"
      {"input_signal_level",	LIBERTY_ATTRENUM_input_signal_level},
#line 318 "attr_lookup"
      {"single_bit_degenerate",	LIBERTY_ATTRENUM_single_bit_degenerate},
#line 104 "attr_lookup"
      {"k_process_rise_delay_intercept",	LIBERTY_ATTRENUM_k_process_rise_delay_intercept},
#line 108 "attr_lookup"
      {"k_process_fall_delay_intercept",	LIBERTY_ATTRENUM_k_process_fall_delay_intercept},
#line 89 "attr_lookup"
      {"k_volt_cell_degradation",	LIBERTY_ATTRENUM_k_volt_cell_degradation},
#line 315 "attr_lookup"
      {"xnf_device_name",	LIBERTY_ATTRENUM_xnf_device_name},
#line 298 "attr_lookup"
      {"driver_type",	LIBERTY_ATTRENUM_driver_type},
#line 449 "attr_lookup"
      {"k_process_nochange_rise",	LIBERTY_ATTRENUM_k_process_nochange_rise},
#line 335 "attr_lookup"
      {"fpga_lut_output",	LIBERTY_ATTRENUM_fpga_lut_output},
#line 450 "attr_lookup"
      {"k_process_nochange_fall",	LIBERTY_ATTRENUM_k_process_nochange_fall},
#line 234 "attr_lookup"
      {"related_rising_pin",	LIBERTY_ATTRENUM_related_rising_pin},
#line 497 "attr_lookup"
      {"default_fanout_load",	LIBERTY_ATTRENUM_default_fanout_load},
#line 367 "attr_lookup"
      {"preferred_output_pad_slew_rate_control",	LIBERTY_ATTRENUM_preferred_output_pad_slew_rate_control},
#line 378 "attr_lookup"
      {"key_feature",	LIBERTY_ATTRENUM_key_feature},
#line 238 "attr_lookup"
      {"related_falling_pin",	LIBERTY_ATTRENUM_related_falling_pin},
#line 287 "attr_lookup"
      {"input_map",	LIBERTY_ATTRENUM_input_map},
#line 190 "attr_lookup"
      {"next_state",	LIBERTY_ATTRENUM_next_state},
#line 427 "attr_lookup"
      {"k_temp_internal_power",	LIBERTY_ATTRENUM_k_temp_internal_power},
#line 138 "attr_lookup"
      {"power_rail",	LIBERTY_ATTRENUM_power_rail},
#line 13 "attr_lookup"
      {"rise_delay_intercept",	LIBERTY_ATTRENUM_rise_delay_intercept},
#line 15 "attr_lookup"
      {"fall_delay_intercept",	LIBERTY_ATTRENUM_fall_delay_intercept},
#line 470 "attr_lookup"
      {"delay_model",	LIBERTY_ATTRENUM_delay_model},
#line 462 "attr_lookup"
      {"k_process_cell_leakage_power",	LIBERTY_ATTRENUM_k_process_cell_leakage_power},
#line 263 "attr_lookup"
      {"pulling_resistance",	LIBERTY_ATTRENUM_pulling_resistance},
#line 250 "attr_lookup"
      {"members",	LIBERTY_ATTRENUM_members},
#line 259 "attr_lookup"
      {"rise_time_after_threshold",	LIBERTY_ATTRENUM_rise_time_after_threshold},
#line 100 "attr_lookup"
      {"k_temp_cell_degradation",	LIBERTY_ATTRENUM_k_temp_cell_degradation},
#line 294 "attr_lookup"
      {"fall_time_after_threshold",	LIBERTY_ATTRENUM_fall_time_after_threshold},
#line 390 "attr_lookup"
      {"k_volt_recovery_rise",	LIBERTY_ATTRENUM_k_volt_recovery_rise},
#line 337 "attr_lookup"
      {"fpga_family",	LIBERTY_ATTRENUM_fpga_family},
#line 391 "attr_lookup"
      {"k_volt_recovery_fall",	LIBERTY_ATTRENUM_k_volt_recovery_fall},
#line 483 "attr_lookup"
      {"default_output_pin_cap",	LIBERTY_ATTRENUM_default_output_pin_cap},
#line 481 "attr_lookup"
      {"default_output_pin_rise_res",	LIBERTY_ATTRENUM_default_output_pin_rise_res},
#line 482 "attr_lookup"
      {"default_output_pin_fall_res",	LIBERTY_ATTRENUM_default_output_pin_fall_res},
#line 82 "attr_lookup"
      {"k_volt_rise_delay_intercept",	LIBERTY_ATTRENUM_k_volt_rise_delay_intercept},
#line 366 "attr_lookup"
      {"preferred_output_pad_voltage",	LIBERTY_ATTRENUM_preferred_output_pad_voltage},
#line 132 "attr_lookup"
      {"default_power_rail",	LIBERTY_ATTRENUM_default_power_rail},
#line 393 "attr_lookup"
      {"k_volt_nochange_rise",	LIBERTY_ATTRENUM_k_volt_nochange_rise},
#line 288 "attr_lookup"
      {"hysteresis",	LIBERTY_ATTRENUM_hysteresis},
#line 394 "attr_lookup"
      {"k_volt_nochange_fall",	LIBERTY_ATTRENUM_k_volt_nochange_fall},
#line 84 "attr_lookup"
      {"k_volt_fall_propagation",	LIBERTY_ATTRENUM_k_volt_fall_propagation},
#line 303 "attr_lookup"
      {"complementary_pin",	LIBERTY_ATTRENUM_complementary_pin},
#line 268 "attr_lookup"
      {"output_voltage",	LIBERTY_ATTRENUM_output_voltage},
#line 417 "attr_lookup"
      {"k_temp_recovery_rise",	LIBERTY_ATTRENUM_k_temp_recovery_rise},
#line 418 "attr_lookup"
      {"k_temp_recovery_fall",	LIBERTY_ATTRENUM_k_temp_recovery_fall},
#line 270 "attr_lookup"
      {"nextstate_type",	LIBERTY_ATTRENUM_nextstate_type},
#line 39 "attr_lookup"
      {"threshold",	LIBERTY_ATTRENUM_threshold},
#line 254 "attr_lookup"
      {"test_output_only",	LIBERTY_ATTRENUM_test_output_only},
#line 93 "attr_lookup"
      {"k_temp_rise_delay_intercept",	LIBERTY_ATTRENUM_k_temp_rise_delay_intercept},
#line 348 "attr_lookup"
      {"auxiliary_pad_cell",	LIBERTY_ATTRENUM_auxiliary_pad_cell},
#line 75 "attr_lookup"
      {"lsi_rounding_digit",	LIBERTY_ATTRENUM_lsi_rounding_digit},
#line 331 "attr_lookup"
      {"handle_negative_constraint",	LIBERTY_ATTRENUM_handle_negative_constraint},
#line 264 "attr_lookup"
      {"pulling_current",	LIBERTY_ATTRENUM_pulling_current},
#line 175 "attr_lookup"
      {"column_address",	LIBERTY_ATTRENUM_column_address},
#line 281 "attr_lookup"
      {"max_capacitance",	LIBERTY_ATTRENUM_max_capacitance},
#line 207 "attr_lookup"
      {"steady_state_resistance_float_min",	LIBERTY_ATTRENUM_steady_state_resistance_float_min},
#line 257 "attr_lookup"
      {"signal_type",	LIBERTY_ATTRENUM_signal_type},
#line 476 "attr_lookup"
      {"default_wire_load",	LIBERTY_ATTRENUM_default_wire_load},
#line 420 "attr_lookup"
      {"k_temp_nochange_rise",	LIBERTY_ATTRENUM_k_temp_nochange_rise},
#line 301 "attr_lookup"
      {"dcm_timing",	LIBERTY_ATTRENUM_dcm_timing},
#line 355 "attr_lookup"
      {"voltage_unit",	LIBERTY_ATTRENUM_voltage_unit},
#line 421 "attr_lookup"
      {"k_temp_nochange_fall",	LIBERTY_ATTRENUM_k_temp_nochange_fall},
#line 475 "attr_lookup"
      {"default_wire_load_area",	LIBERTY_ATTRENUM_default_wire_load_area},
#line 95 "attr_lookup"
      {"k_temp_fall_propagation",	LIBERTY_ATTRENUM_k_temp_fall_propagation},
#line 153 "attr_lookup"
      {"variable_5_range",	LIBERTY_ATTRENUM_variable_5_range},
#line 261 "attr_lookup"
      {"rise_current_slope_after_threshold",	LIBERTY_ATTRENUM_rise_current_slope_after_threshold},
#line 156 "attr_lookup"
      {"variable_2_range",	LIBERTY_ATTRENUM_variable_2_range},
#line 296 "attr_lookup"
      {"fall_current_slope_after_threshold",	LIBERTY_ATTRENUM_fall_current_slope_after_threshold},
#line 38 "attr_lookup"
      {"variable_7_range",	LIBERTY_ATTRENUM_variable_7_range},
#line 197 "attr_lookup"
      {"edge_type",	LIBERTY_ATTRENUM_edge_type},
#line 365 "attr_lookup"
      {"pulling_resistance_unit",	LIBERTY_ATTRENUM_pulling_resistance_unit},
#line 154 "attr_lookup"
      {"variable_4_range",	LIBERTY_ATTRENUM_variable_4_range},
#line 152 "attr_lookup"
      {"variable_6_range",	LIBERTY_ATTRENUM_variable_6_range},
#line 406 "attr_lookup"
      {"k_volt_cell_leakage_power",	LIBERTY_ATTRENUM_k_volt_cell_leakage_power},
#line 127 "attr_lookup"
      {"fanout_length",	LIBERTY_ATTRENUM_fanout_length},
#line 338 "attr_lookup"
      {"fpga_complex_degenerate",	LIBERTY_ATTRENUM_fpga_complex_degenerate},
#line 472 "attr_lookup"
      {"default_wire_load_resistance",	LIBERTY_ATTRENUM_default_wire_load_resistance},
#line 157 "attr_lookup"
      {"variable_1_range",	LIBERTY_ATTRENUM_variable_1_range},
#line 471 "attr_lookup"
      {"default_wire_load_selection",	LIBERTY_ATTRENUM_default_wire_load_selection},
#line 474 "attr_lookup"
      {"default_wire_load_capacitance",	LIBERTY_ATTRENUM_default_wire_load_capacitance},
#line 155 "attr_lookup"
      {"variable_3_range",	LIBERTY_ATTRENUM_variable_3_range},
#line 304 "attr_lookup"
      {"clock_gate_test_pin",	LIBERTY_ATTRENUM_clock_gate_test_pin},
#line 489 "attr_lookup"
      {"default_max_capacitance",	LIBERTY_ATTRENUM_default_max_capacitance},
#line 172 "attr_lookup"
      {"word_width",	LIBERTY_ATTRENUM_word_width},
#line 327 "attr_lookup"
      {"mpm_libname",	LIBERTY_ATTRENUM_mpm_libname},
#line 43 "attr_lookup"
      {"output_signal_level_low",	LIBERTY_ATTRENUM_output_signal_level_low},
#line 290 "attr_lookup"
      {"fpga_degenerate_output",	LIBERTY_ATTRENUM_fpga_degenerate_output},
#line 17 "attr_lookup"
      {"steady_state_resistance_low",	LIBERTY_ATTRENUM_steady_state_resistance_low},
#line 487 "attr_lookup"
      {"default_max_transition",	LIBERTY_ATTRENUM_default_max_transition},
#line 179 "attr_lookup"
      {"enable_also",	LIBERTY_ATTRENUM_enable_also},
#line 45 "attr_lookup"
      {"min_input_noise_width",	LIBERTY_ATTRENUM_min_input_noise_width},
#line 339 "attr_lookup"
      {"fpga_cell_type",	LIBERTY_ATTRENUM_fpga_cell_type},
#line 433 "attr_lookup"
      {"k_temp_cell_leakage_power",	LIBERTY_ATTRENUM_k_temp_cell_leakage_power},
#line 317 "attr_lookup"
      {"use_for_size_only",	LIBERTY_ATTRENUM_use_for_size_only},
#line 117 "attr_lookup"
      {"default_hold_coefficient",	LIBERTY_ATTRENUM_default_hold_coefficient},
#line 308 "attr_lookup"
      {"clock_gate_clock_pin",	LIBERTY_ATTRENUM_clock_gate_clock_pin},
#line 333 "attr_lookup"
      {"fpga_min_degen_input_size",	LIBERTY_ATTRENUM_fpga_min_degen_input_size},
#line 484 "attr_lookup"
      {"default_operating_conditions",	LIBERTY_ATTRENUM_default_operating_conditions},
#line 126 "attr_lookup"
      {"wire_load_from_area",	LIBERTY_ATTRENUM_wire_load_from_area},
#line 22 "attr_lookup"
      {"input_signal_level_low",	LIBERTY_ATTRENUM_input_signal_level_low},
#line 325 "attr_lookup"
      {"observe_node",	LIBERTY_ATTRENUM_observe_node},
#line 221 "attr_lookup"
      {"default_timing",	LIBERTY_ATTRENUM_default_timing},
#line 249 "attr_lookup"
      {"related_bus_pins",	LIBERTY_ATTRENUM_related_bus_pins},
#line 305 "attr_lookup"
      {"clock_gate_out_pin",	LIBERTY_ATTRENUM_clock_gate_out_pin},
#line 164 "attr_lookup"
      {"base_type",	LIBERTY_ATTRENUM_base_type},
#line 232 "attr_lookup"
      {"switching_interval",	LIBERTY_ATTRENUM_switching_interval},
#line 488 "attr_lookup"
      {"default_max_fanout",	LIBERTY_ATTRENUM_default_max_fanout},
#line 379 "attr_lookup"
      {"key_bit",	LIBERTY_ATTRENUM_key_bit},
#line 35 "attr_lookup"
      {"height_coefficient",	LIBERTY_ATTRENUM_height_coefficient},
#line 59 "attr_lookup"
      {"edge_rate_breakpoint_r0",	LIBERTY_ATTRENUM_edge_rate_breakpoint_r0},
#line 61 "attr_lookup"
      {"edge_rate_breakpoint_f0",	LIBERTY_ATTRENUM_edge_rate_breakpoint_f0},
#line 201 "attr_lookup"
      {"timing_type",	LIBERTY_ATTRENUM_timing_type},
#line 63 "attr_lookup"
      {"timing_model_type",	LIBERTY_ATTRENUM_timing_model_type},
#line 486 "attr_lookup"
      {"default_max_utilization",	LIBERTY_ATTRENUM_default_max_utilization},
#line 369 "attr_lookup"
      {"output_threshold_pct_rise",	LIBERTY_ATTRENUM_output_threshold_pct_rise},
#line 370 "attr_lookup"
      {"output_threshold_pct_fall",	LIBERTY_ATTRENUM_output_threshold_pct_fall},
#line 195 "attr_lookup"
      {"bus_type",	LIBERTY_ATTRENUM_bus_type},
#line 208 "attr_lookup"
      {"steady_state_resistance_float_max",	LIBERTY_ATTRENUM_steady_state_resistance_float_max},
#line 251 "attr_lookup"
      {"input_map_shift",	LIBERTY_ATTRENUM_input_map_shift},
#line 58 "attr_lookup"
      {"edge_rate_breakpoint_r1",	LIBERTY_ATTRENUM_edge_rate_breakpoint_r1},
#line 26 "attr_lookup"
      {"edge_rate_sensitivity_r0",	LIBERTY_ATTRENUM_edge_rate_sensitivity_r0},
#line 60 "attr_lookup"
      {"edge_rate_breakpoint_f1",	LIBERTY_ATTRENUM_edge_rate_breakpoint_f1},
#line 28 "attr_lookup"
      {"edge_rate_sensitivity_f0",	LIBERTY_ATTRENUM_edge_rate_sensitivity_f0},
#line 199 "attr_lookup"
      {"when_start",	LIBERTY_ATTRENUM_when_start},
#line 86 "attr_lookup"
      {"k_volt_fall_delay_intercept",	LIBERTY_ATTRENUM_k_volt_fall_delay_intercept},
#line 473 "attr_lookup"
      {"default_wire_load_mode",	LIBERTY_ATTRENUM_default_wire_load_mode},
#line 374 "attr_lookup"
      {"leakage_power_unit",	LIBERTY_ATTRENUM_leakage_power_unit},
#line 350 "attr_lookup"
      {"technology",	LIBERTY_ATTRENUM_technology},
#line 203 "attr_lookup"
      {"steady_state_resistance_low_min",	LIBERTY_ATTRENUM_steady_state_resistance_low_min},
#line 346 "attr_lookup"
      {"cell_leakage_power",	LIBERTY_ATTRENUM_cell_leakage_power},
#line 53 "attr_lookup"
      {"has_builtin_pad",	LIBERTY_ATTRENUM_has_builtin_pad},
#line 219 "attr_lookup"
      {"fpga_timing_type",	LIBERTY_ATTRENUM_fpga_timing_type},
#line 176 "attr_lookup"
      {"address_width",	LIBERTY_ATTRENUM_address_width},
#line 25 "attr_lookup"
      {"edge_rate_sensitivity_r1",	LIBERTY_ATTRENUM_edge_rate_sensitivity_r1},
#line 27 "attr_lookup"
      {"edge_rate_sensitivity_f1",	LIBERTY_ATTRENUM_edge_rate_sensitivity_f1},
#line 480 "attr_lookup"
      {"default_rise_delay_intercept",	LIBERTY_ATTRENUM_default_rise_delay_intercept},
#line 499 "attr_lookup"
      {"default_fall_delay_intercept",	LIBERTY_ATTRENUM_default_fall_delay_intercept},
#line 97 "attr_lookup"
      {"k_temp_fall_delay_intercept",	LIBERTY_ATTRENUM_k_temp_fall_delay_intercept},
#line 358 "attr_lookup"
      {"slew_upper_threshold_pct_rise",	LIBERTY_ATTRENUM_slew_upper_threshold_pct_rise},
#line 226 "attr_lookup"
      {"constraint_high",	LIBERTY_ATTRENUM_constraint_high},
#line 46 "attr_lookup"
      {"max_input_noise_width",	LIBERTY_ATTRENUM_max_input_noise_width},
#line 359 "attr_lookup"
      {"slew_upper_threshold_pct_fall",	LIBERTY_ATTRENUM_slew_upper_threshold_pct_fall},
#line 469 "attr_lookup"
      {"em_temp_degradation_factor",	LIBERTY_ATTRENUM_em_temp_degradation_factor},
#line 336 "attr_lookup"
      {"fpga_lut_insert_before_sequential",	LIBERTY_ATTRENUM_fpga_lut_insert_before_sequential},
#line 501 "attr_lookup"
      {"default_cell_leakage_power",	LIBERTY_ATTRENUM_default_cell_leakage_power},
#line 466 "attr_lookup"
      {"fpga_prefer_undegenerated_gates",	LIBERTY_ATTRENUM_fpga_prefer_undegenerated_gates},
#line 186 "attr_lookup"
      {"duty_cycle",	LIBERTY_ATTRENUM_duty_cycle},
#line 485 "attr_lookup"
      {"default_min_porosity",	LIBERTY_ATTRENUM_default_min_porosity},
#line 265 "attr_lookup"
      {"primary_output",	LIBERTY_ATTRENUM_primary_output},
#line 273 "attr_lookup"
      {"min_pulse_width_low",	LIBERTY_ATTRENUM_min_pulse_width_low},
#line 161 "attr_lookup"
      {"bit_width",	LIBERTY_ATTRENUM_bit_width},
#line 258 "attr_lookup"
      {"rise_time_before_threshold",	LIBERTY_ATTRENUM_rise_time_before_threshold},
#line 360 "attr_lookup"
      {"slew_lower_threshold_pct_rise",	LIBERTY_ATTRENUM_slew_lower_threshold_pct_rise},
#line 293 "attr_lookup"
      {"fall_time_before_threshold",	LIBERTY_ATTRENUM_fall_time_before_threshold},
#line 361 "attr_lookup"
      {"slew_lower_threshold_pct_fall",	LIBERTY_ATTRENUM_slew_lower_threshold_pct_fall},
#line 351 "attr_lookup"
      {"routing_layers",	LIBERTY_ATTRENUM_routing_layers},
#line 395 "attr_lookup"
      {"k_volt_min_pulse_width_low",	LIBERTY_ATTRENUM_k_volt_min_pulse_width_low},
#line 216 "attr_lookup"
      {"related_bus_equivalent",	LIBERTY_ATTRENUM_related_bus_equivalent},
#line 279 "attr_lookup"
      {"max_time_borrow",	LIBERTY_ATTRENUM_max_time_borrow},
#line 422 "attr_lookup"
      {"k_temp_min_pulse_width_low",	LIBERTY_ATTRENUM_k_temp_min_pulse_width_low},
#line 451 "attr_lookup"
      {"k_process_min_pulse_width_low",	LIBERTY_ATTRENUM_k_process_min_pulse_width_low},
#line 204 "attr_lookup"
      {"steady_state_resistance_low_max",	LIBERTY_ATTRENUM_steady_state_resistance_low_max},
#line 260 "attr_lookup"
      {"rise_current_slope_before_threshold",	LIBERTY_ATTRENUM_rise_current_slope_before_threshold},
#line 295 "attr_lookup"
      {"fall_current_slope_before_threshold",	LIBERTY_ATTRENUM_fall_current_slope_before_threshold},
#line 307 "attr_lookup"
      {"clock_gate_enable_pin",	LIBERTY_ATTRENUM_clock_gate_enable_pin},
#line 306 "attr_lookup"
      {"clock_gate_obs_pin",	LIBERTY_ATTRENUM_clock_gate_obs_pin},
#line 41 "attr_lookup"
      {"power_gating_pin",	LIBERTY_ATTRENUM_power_gating_pin},
#line 65 "attr_lookup"
      {"power_gating_cell",	LIBERTY_ATTRENUM_power_gating_cell},
#line 352 "attr_lookup"
      {"library_features",	LIBERTY_ATTRENUM_library_features},
#line 233 "attr_lookup"
      {"rising_together_group",	LIBERTY_ATTRENUM_rising_together_group},
#line 334 "attr_lookup"
      {"fpga_max_degen_input_size",	LIBERTY_ATTRENUM_fpga_max_degen_input_size},
#line 340 "attr_lookup"
      {"fpga_bridge_inputs",	LIBERTY_ATTRENUM_fpga_bridge_inputs},
#line 29 "attr_lookup"
      {"clock_gating_flag",	LIBERTY_ATTRENUM_clock_gating_flag},
#line 183 "attr_lookup"
      {"multiplied_by",	LIBERTY_ATTRENUM_multiplied_by},
#line 362 "attr_lookup"
      {"slew_derate_from_library",	LIBERTY_ATTRENUM_slew_derate_from_library},
#line 119 "attr_lookup"
      {"default_edge_rate_breakpoint_r0",	LIBERTY_ATTRENUM_default_edge_rate_breakpoint_r0},
#line 121 "attr_lookup"
      {"default_edge_rate_breakpoint_f0",	LIBERTY_ATTRENUM_default_edge_rate_breakpoint_f0},
#line 118 "attr_lookup"
      {"default_edge_rate_breakpoint_r1",	LIBERTY_ATTRENUM_default_edge_rate_breakpoint_r1},
#line 120 "attr_lookup"
      {"default_edge_rate_breakpoint_f1",	LIBERTY_ATTRENUM_default_edge_rate_breakpoint_f1},
#line 44 "attr_lookup"
      {"output_signal_level_high",	LIBERTY_ATTRENUM_output_signal_level_high},
#line 187 "attr_lookup"
      {"divided_by",	LIBERTY_ATTRENUM_divided_by},
#line 18 "attr_lookup"
      {"steady_state_resistance_high",	LIBERTY_ATTRENUM_steady_state_resistance_high},
#line 345 "attr_lookup"
      {"clock_gating_integrated_cell",	LIBERTY_ATTRENUM_clock_gating_integrated_cell},
#line 23 "attr_lookup"
      {"input_signal_level_high",	LIBERTY_ATTRENUM_input_signal_level_high},
#line 64 "attr_lookup"
      {"threshold_voltage_group",	LIBERTY_ATTRENUM_threshold_voltage_group},
#line 467 "attr_lookup"
      {"fpga_allow_duplicate_degenerated_gates",	LIBERTY_ATTRENUM_fpga_allow_duplicate_degenerated_gates},
#line 239 "attr_lookup"
      {"falling_together_group",	LIBERTY_ATTRENUM_falling_together_group},
#line 19 "attr_lookup"
      {"steady_state_resistance_below_low",	LIBERTY_ATTRENUM_steady_state_resistance_below_low},
#line 505 "attr_lookup"
      {"bus_naming_style",	LIBERTY_ATTRENUM_bus_naming_style},
#line 205 "attr_lookup"
      {"steady_state_resistance_high_min",	LIBERTY_ATTRENUM_steady_state_resistance_high_min},
#line 490 "attr_lookup"
      {"default_leakage_power_density",	LIBERTY_ATTRENUM_default_leakage_power_density},
#line 274 "attr_lookup"
      {"min_pulse_width_high",	LIBERTY_ATTRENUM_min_pulse_width_high},
#line 68 "attr_lookup"
      {"ignore_verify_icg_type",	LIBERTY_ATTRENUM_ignore_verify_icg_type},
#line 396 "attr_lookup"
      {"k_volt_min_pulse_width_high",	LIBERTY_ATTRENUM_k_volt_min_pulse_width_high},
#line 423 "attr_lookup"
      {"k_temp_min_pulse_width_high",	LIBERTY_ATTRENUM_k_temp_min_pulse_width_high},
#line 452 "attr_lookup"
      {"k_process_min_pulse_width_high",	LIBERTY_ATTRENUM_k_process_min_pulse_width_high},
#line 206 "attr_lookup"
      {"steady_state_resistance_high_max",	LIBERTY_ATTRENUM_steady_state_resistance_high_max},
#line 231 "attr_lookup"
      {"switching_together_group",	LIBERTY_ATTRENUM_switching_together_group},
#line 112 "attr_lookup"
      {"default_threshold_voltage_group",	LIBERTY_ATTRENUM_default_threshold_voltage_group},
#line 20 "attr_lookup"
      {"steady_state_resistance_above_high",	LIBERTY_ATTRENUM_steady_state_resistance_above_high}
    };

  static const short lookup[] =
    {
       -1,  -1,  -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,
        1,  -1,  -1,  -1,  -1,   2,  -1,  -1,  -1,  -1,
       -1,  -1,   3,  -1,  -1,   4,  -1,  -1,  -1,   5,
       -1,  -1,  -1,  -1,  -1,  -1,   6,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,   7,  -1,  -1,   8,  -1,
       -1,  -1,   9,  -1,  -1,  10,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  11,
       -1,  -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,  -1,
       13,  14,  -1,  -1,  -1,  15,  -1,  -1,  16,  -1,
       17,  18,  19,  -1,  20,  -1,  21,  -1,  -1,  22,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       23,  -1,  -1,  24,  25,  -1,  26,  27,  28,  -1,
       -1,  29,  -1,  30,  -1,  -1,  31,  32,  -1,  33,
       -1,  -1,  -1,  34,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  35,  -1,  -1,  -1,  -1,  36,  -1,
       -1,  -1,  -1,  37,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  38,  -1,  -1,  -1,  -1,  39,  -1,  -1,  40,
       -1,  -1,  41,  -1,  42,  43,  -1,  44,  -1,  -1,
       45,  -1,  46,  -1,  -1,  -1,  -1,  47,  -1,  48,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  49,  -1,
       -1,  -1,  -1,  50,  -1,  -1,  51,  52,  -1,  -1,
       -1,  -1,  -1,  -1,  53,  -1,  54,  -1,  -1,  55,
       -1,  -1,  56,  -1,  57,  58,  -1,  -1,  -1,  59,
       -1,  -1,  60,  -1,  -1,  -1,  61,  62,  -1,  63,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  64,  65,  -1,
       66,  -1,  -1,  -1,  -1,  67,  -1,  -1,  -1,  68,
       69,  70,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       71,  -1,  -1,  -1,  -1,  72,  -1,  -1,  73,  -1,
       74,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       75,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  76,  -1,  -1,  -1,  77,
       78,  -1,  -1,  79,  80,  81,  -1,  82,  -1,  83,
       84,  -1,  -1,  -1,  85,  -1,  -1,  86,  -1,  -1,
       87,  -1,  88,  89,  -1,  90,  91,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  92,  93,  -1,  -1,  94,
       95,  -1,  -1,  -1,  96,  97,  98,  -1,  -1,  99,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 100,  -1,
       -1,  -1, 101,  -1,  -1,  -1, 102,  -1,  -1,  -1,
       -1, 103,  -1,  -1,  -1, 104, 105,  -1,  -1, 106,
      107, 108,  -1,  -1,  -1,  -1,  -1, 109, 110,  -1,
      111,  -1, 112,  -1,  -1,  -1,  -1, 113,  -1,  -1,
       -1,  -1, 114,  -1,  -1,  -1,  -1, 115,  -1,  -1,
       -1, 116, 117,  -1, 118,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 119,  -1, 120,  -1,  -1,
      121, 122, 123,  -1,  -1,  -1, 124, 125,  -1,  -1,
       -1, 126,  -1,  -1, 127,  -1, 128,  -1, 129,  -1,
       -1,  -1, 130,  -1,  -1,  -1,  -1, 131,  -1,  -1,
       -1, 132, 133,  -1,  -1, 134,  -1, 135,  -1,  -1,
       -1,  -1, 136, 137,  -1,  -1,  -1, 138,  -1,  -1,
      139,  -1,  -1, 140,  -1,  -1, 141,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 142, 143,  -1,  -1, 144,
       -1,  -1, 145,  -1,  -1,  -1,  -1, 146,  -1, 147,
       -1,  -1,  -1,  -1,  -1, 148,  -1,  -1,  -1,  -1,
       -1, 149, 150,  -1,  -1,  -1, 151, 152,  -1,  -1,
       -1,  -1, 153, 154,  -1,  -1, 155, 156,  -1, 157,
       -1,  -1, 158, 159,  -1, 160, 161, 162,  -1,  -1,
       -1,  -1,  -1,  -1, 163, 164,  -1,  -1,  -1,  -1,
      165,  -1, 166,  -1,  -1,  -1,  -1, 167,  -1,  -1,
      168,  -1, 169,  -1,  -1, 170,  -1, 171,  -1, 172,
       -1, 173,  -1,  -1,  -1,  -1, 174,  -1,  -1, 175,
       -1,  -1,  -1,  -1, 176,  -1, 177,  -1,  -1, 178,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      179,  -1, 180,  -1, 181,  -1,  -1, 182,  -1,  -1,
      183, 184, 185,  -1, 186,  -1, 187,  -1,  -1, 188,
      189,  -1,  -1, 190, 191, 192,  -1,  -1, 193, 194,
      195,  -1, 196,  -1,  -1,  -1,  -1, 197, 198,  -1,
      199,  -1,  -1,  -1,  -1,  -1, 200,  -1,  -1,  -1,
       -1, 201,  -1,  -1, 202, 203,  -1, 204,  -1,  -1,
       -1,  -1, 205,  -1,  -1,  -1,  -1, 206,  -1, 207,
       -1, 208, 209,  -1,  -1, 210,  -1, 211,  -1,  -1,
      212,  -1, 213, 214,  -1,  -1,  -1,  -1, 215, 216,
      217,  -1,  -1,  -1, 218, 219,  -1,  -1,  -1, 220,
      221, 222,  -1,  -1,  -1,  -1,  -1,  -1, 223,  -1,
       -1,  -1,  -1,  -1, 224,  -1,  -1, 225, 226,  -1,
       -1,  -1,  -1, 227,  -1, 228,  -1, 229,  -1,  -1,
       -1, 230,  -1, 231, 232,  -1,  -1,  -1,  -1, 233,
       -1,  -1,  -1, 234,  -1, 235,  -1,  -1, 236,  -1,
       -1,  -1,  -1, 237, 238, 239,  -1, 240,  -1, 241,
      242,  -1,  -1,  -1, 243,  -1, 244, 245,  -1,  -1,
       -1, 246, 247,  -1,  -1, 248, 249,  -1,  -1,  -1,
      250,  -1, 251,  -1,  -1,  -1, 252, 253,  -1,  -1,
       -1,  -1, 254,  -1, 255,  -1,  -1, 256,  -1, 257,
       -1,  -1, 258,  -1,  -1,  -1, 259, 260, 261, 262,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 263,  -1, 264,
       -1,  -1, 265,  -1,  -1,  -1, 266,  -1, 267,  -1,
       -1,  -1, 268,  -1, 269,  -1, 270,  -1,  -1, 271,
       -1, 272, 273,  -1, 274,  -1,  -1, 275,  -1, 276,
       -1,  -1,  -1,  -1,  -1,  -1, 277,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 278,  -1,  -1, 279,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 280,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 281,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 282,  -1,  -1,
      283, 284,  -1, 285,  -1, 286,  -1,  -1,  -1,  -1,
      287, 288,  -1,  -1,  -1,  -1, 289, 290, 291,  -1,
      292, 293,  -1,  -1,  -1,  -1,  -1, 294,  -1,  -1,
       -1,  -1,  -1,  -1, 295,  -1,  -1,  -1,  -1,  -1,
      296,  -1,  -1,  -1,  -1, 297,  -1,  -1, 298,  -1,
      299,  -1,  -1, 300,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 301, 302,  -1,  -1,
       -1,  -1, 303, 304,  -1, 305,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 306,  -1,
       -1, 307,  -1,  -1,  -1, 308, 309,  -1, 310, 311,
       -1, 312,  -1, 313, 314,  -1, 315, 316,  -1, 317,
      318,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 319,  -1,  -1,  -1,  -1, 320, 321,  -1,
       -1, 322,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 323, 324,  -1, 325,  -1, 326,  -1,
       -1, 327,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 328, 329,  -1,  -1,  -1, 330,  -1,  -1,
       -1,  -1, 331,  -1,  -1,  -1,  -1, 332,  -1,  -1,
       -1,  -1, 333,  -1,  -1,  -1,  -1,  -1,  -1, 334,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 335,  -1,
       -1, 336,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 337,  -1,
      338,  -1,  -1, 339,  -1,  -1,  -1,  -1, 340, 341,
       -1,  -1, 342,  -1,  -1,  -1,  -1, 343,  -1,  -1,
       -1,  -1, 344,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 345,  -1,  -1,  -1,
      346,  -1,  -1, 347, 348, 349,  -1,  -1, 350,  -1,
       -1,  -1,  -1,  -1, 351, 352, 353,  -1,  -1,  -1,
       -1,  -1, 354,  -1,  -1, 355,  -1,  -1, 356,  -1,
      357,  -1,  -1, 358,  -1, 359,  -1, 360, 361,  -1,
      362,  -1,  -1, 363,  -1,  -1,  -1,  -1, 364,  -1,
      365,  -1,  -1, 366,  -1, 367,  -1,  -1, 368,  -1,
       -1, 369,  -1, 370,  -1,  -1,  -1,  -1, 371,  -1,
       -1,  -1,  -1, 372,  -1,  -1,  -1,  -1, 373,  -1,
       -1, 374,  -1, 375,  -1,  -1, 376,  -1,  -1,  -1,
       -1,  -1,  -1, 377,  -1, 378,  -1, 379, 380,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 381,  -1,  -1,  -1,
       -1,  -1,  -1, 382, 383,  -1,  -1,  -1, 384,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 385,  -1,  -1,  -1,
      386,  -1,  -1,  -1,  -1,  -1,  -1, 387,  -1,  -1,
       -1,  -1, 388,  -1,  -1, 389,  -1, 390,  -1,  -1,
       -1, 391,  -1, 392, 393,  -1,  -1, 394,  -1,  -1,
       -1,  -1, 395,  -1,  -1, 396,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 397,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 398,
      399,  -1,  -1,  -1, 400,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 401,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 402,  -1,
      403,  -1,  -1, 404,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 405,
       -1,  -1,  -1, 406,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 407,  -1,  -1, 408, 409,  -1,  -1,  -1,  -1,
      410,  -1,  -1, 411,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 412,  -1,  -1,  -1, 413,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 414,  -1,  -1,  -1,
       -1, 415,  -1,  -1,  -1,  -1,  -1, 416,  -1,  -1,
      417,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 418,  -1,  -1,  -1,  -1,
      419, 420,  -1,  -1,  -1, 421, 422,  -1, 423,  -1,
       -1,  -1,  -1,  -1, 424,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 425,  -1,  -1,  -1,  -1,
       -1, 426,  -1,  -1,  -1, 427, 428,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 429,  -1,
      430,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 431,  -1,
       -1,  -1,  -1,  -1, 432,  -1, 433,  -1,  -1,  -1,
       -1, 434,  -1,  -1,  -1, 435,  -1,  -1,  -1,  -1,
      436,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 437,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 438,  -1,
       -1, 439, 440, 441,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 442,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 443,  -1,  -1,  -1, 444,
       -1, 445,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 446,
       -1,  -1, 447,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 448,  -1, 449,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 450,  -1,  -1,
       -1,  -1,  -1,  -1, 451,  -1,  -1,  -1, 452, 453,
       -1,  -1,  -1, 454,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 455,  -1,  -1,  -1,
      456,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 457,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 458,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 459,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 460,  -1,
       -1, 461,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 462,  -1,
       -1,  -1,  -1, 463,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      464,  -1, 465,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 466, 467,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      468,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 469,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 470,  -1,  -1, 471,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 472,  -1,  -1, 473,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 474,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 475,  -1,
       -1,  -1,  -1, 476,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 477,  -1,  -1,  -1,  -1, 478,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      479,  -1,  -1,  -1, 480,  -1,  -1,  -1,  -1,  -1,
       -1, 481,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 482,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 483,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 484,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 485,  -1,  -1,  -1,
      486,  -1,  -1, 487,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      488,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      489,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 490,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 491,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 492,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 493,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 494,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 495,  -1,  -1, 496,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 497,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 498,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 499
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = attr_hash_func (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
#line 506 "attr_lookup"

END_NAMESPACE_ADS
