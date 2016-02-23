/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __MSMB_ISP__
#define __MSMB_ISP__
#include <linux/videodev2.h>
#define MAX_PLANES_PER_STREAM 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAX_NUM_STREAM 7
#define ISP_VERSION_47 47
#define ISP_VERSION_46 46
#define ISP_VERSION_44 44
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_VERSION_40 40
#define ISP_VERSION_32 32
#define ISP_NATIVE_BUF_BIT (0x10000 << 0)
#define ISP0_BIT (0x10000 << 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP1_BIT (0x10000 << 2)
#define ISP_META_CHANNEL_BIT (0x10000 << 3)
#define ISP_SCRATCH_BUF_BIT (0x10000 << 4)
#define ISP_STATS_STREAM_BIT 0x80000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_cfg_cmd_list;
enum ISP_START_PIXEL_PATTERN {
  ISP_BAYER_RGRGRG,
  ISP_BAYER_GRGRGR,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_BAYER_BGBGBG,
  ISP_BAYER_GBGBGB,
  ISP_YUV_YCbYCr,
  ISP_YUV_YCrYCb,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_YUV_CbYCrY,
  ISP_YUV_CrYCbY,
  ISP_PIX_PATTERN_MAX
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_plane_fmt {
  Y_PLANE,
  CB_PLANE,
  CR_PLANE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CRCB_PLANE,
  CBCR_PLANE,
  VFE_PLANE_FMT_MAX
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_input_src {
  VFE_PIX_0,
  VFE_RAW_0,
  VFE_RAW_1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_RAW_2,
  VFE_SRC_MAX,
};
enum msm_vfe_axi_stream_src {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PIX_ENCODER,
  PIX_VIEWFINDER,
  PIX_VIDEO,
  CAMIF_RAW,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  IDEAL_RAW,
  RDI_INTF_0,
  RDI_INTF_1,
  RDI_INTF_2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_AXI_SRC_MAX
};
enum msm_vfe_frame_skip_pattern {
  NO_SKIP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  EVERY_2FRAME,
  EVERY_3FRAME,
  EVERY_4FRAME,
  EVERY_5FRAME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  EVERY_6FRAME,
  EVERY_7FRAME,
  EVERY_8FRAME,
  EVERY_16FRAME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  EVERY_32FRAME,
  SKIP_ALL,
  SKIP_RANGE,
  MAX_SKIP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_isp_stats_type {
  MSM_ISP_STATS_AEC,
  MSM_ISP_STATS_AF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_AWB,
  MSM_ISP_STATS_RS,
  MSM_ISP_STATS_CS,
  MSM_ISP_STATS_IHIST,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_SKIN,
  MSM_ISP_STATS_BG,
  MSM_ISP_STATS_BF,
  MSM_ISP_STATS_BE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_BHIST,
  MSM_ISP_STATS_BF_SCALE,
  MSM_ISP_STATS_HDR_BE,
  MSM_ISP_STATS_HDR_BHIST,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_AEC_BG,
  MSM_ISP_STATS_MAX
};
struct msm_isp_sw_framskip {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stats_type_mask;
  uint32_t stream_src_mask;
  enum msm_vfe_frame_skip_pattern skip_mode;
  uint32_t min_frame_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t max_frame_id;
};
enum msm_vfe_testgen_color_pattern {
  COLOR_BAR_8_COLOR,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UNICOLOR_WHITE,
  UNICOLOR_YELLOW,
  UNICOLOR_CYAN,
  UNICOLOR_GREEN,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UNICOLOR_MAGENTA,
  UNICOLOR_RED,
  UNICOLOR_BLUE,
  UNICOLOR_BLACK,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MAX_COLOR,
};
enum msm_vfe_camif_input {
  CAMIF_DISABLED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CAMIF_PAD_REG_INPUT,
  CAMIF_MIDDI_INPUT,
  CAMIF_MIPI_INPUT,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_fetch_engine_cfg {
  uint32_t input_format;
  uint32_t buf_width;
  uint32_t buf_height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t fetch_width;
  uint32_t fetch_height;
  uint32_t x_offset;
  uint32_t y_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t buf_stride;
};
struct msm_vfe_camif_subsample_cfg {
  uint32_t irq_subsample_period;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t irq_subsample_pattern;
  uint32_t sof_counter_step;
  uint32_t pixel_skip;
  uint32_t line_skip;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_camif_cfg {
  uint32_t lines_per_frame;
  uint32_t pixels_per_line;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t first_pixel;
  uint32_t last_pixel;
  uint32_t first_line;
  uint32_t last_line;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t epoch_line0;
  uint32_t epoch_line1;
  enum msm_vfe_camif_input camif_input;
  struct msm_vfe_camif_subsample_cfg subsample_cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_testgen_cfg {
  uint32_t lines_per_frame;
  uint32_t pixels_per_line;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t v_blank;
  uint32_t h_blank;
  enum ISP_START_PIXEL_PATTERN pixel_bayer_pattern;
  uint32_t rotate_period;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_testgen_color_pattern color_bar_pattern;
  uint32_t burst_num_frame;
};
enum msm_vfe_inputmux {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CAMIF,
  TESTGEN,
  EXTERNAL_READ,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_stats_composite_group {
  STATS_COMPOSITE_GRP_NONE,
  STATS_COMPOSITE_GRP_1,
  STATS_COMPOSITE_GRP_2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  STATS_COMPOSITE_GRP_MAX,
};
struct msm_vfe_pix_cfg {
  struct msm_vfe_camif_cfg camif_cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_vfe_testgen_cfg testgen_cfg;
  struct msm_vfe_fetch_engine_cfg fetch_engine_cfg;
  enum msm_vfe_inputmux input_mux;
  enum ISP_START_PIXEL_PATTERN pixel_pattern;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t input_format;
  uint32_t is_split;
};
struct msm_vfe_rdi_cfg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t cid;
  uint8_t frame_based;
};
struct msm_vfe_input_cfg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  union {
    struct msm_vfe_pix_cfg pix_cfg;
    struct msm_vfe_rdi_cfg rdi_cfg;
  } d;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_input_src input_src;
  uint32_t input_pix_clk;
};
struct msm_vfe_fetch_eng_start {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
  uint32_t buf_idx;
  uint32_t buf_addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_axi_plane_cfg {
  uint32_t output_width;
  uint32_t output_height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t output_stride;
  uint32_t output_scan_lines;
  uint32_t output_plane_format;
  uint32_t plane_addr_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t csid_src;
  uint8_t rdi_cid;
};
enum msm_stream_memory_input_t {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MEMORY_INPUT_DISABLED,
  MEMORY_INPUT_ENABLED
};
struct msm_vfe_axi_stream_request_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
  uint32_t vt_enable;
  uint32_t output_format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_axi_stream_src stream_src;
  struct msm_vfe_axi_plane_cfg plane_cfg[MAX_PLANES_PER_STREAM];
  uint32_t burst_count;
  uint32_t hfr_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t frame_base;
  uint32_t init_frame_drop;
  enum msm_vfe_frame_skip_pattern frame_skip_pattern;
  uint8_t buf_divert;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t axi_stream_handle;
  uint32_t controllable_output;
  uint32_t burst_len;
  enum msm_stream_memory_input_t memory_input;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_axi_stream_release_cmd {
  uint32_t stream_handle;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_axi_stream_cmd {
  STOP_STREAM,
  START_STREAM,
  STOP_IMMEDIATELY,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_axi_stream_cfg_cmd {
  uint8_t num_streams;
  uint32_t stream_handle[VFE_AXI_SRC_MAX];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_axi_stream_cmd cmd;
};
enum msm_vfe_axi_stream_update_type {
  ENABLE_STREAM_BUF_DIVERT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  DISABLE_STREAM_BUF_DIVERT,
  UPDATE_STREAM_FRAMEDROP_PATTERN,
  UPDATE_STREAM_STATS_FRAMEDROP_PATTERN,
  UPDATE_STREAM_AXI_CONFIG,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UPDATE_STREAM_REQUEST_FRAMES,
  UPDATE_STREAM_ADD_BUFQ,
  UPDATE_STREAM_REMOVE_BUFQ,
  UPDATE_STREAM_SW_FRAME_DROP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_iommu_type {
  IOMMU_ATTACH,
  IOMMU_DETACH,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_buff_queue_id {
  VFE_BUF_QUEUE_DEFAULT,
  VFE_BUF_QUEUE_SHARED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_BUF_QUEUE_MAX,
};
struct msm_vfe_axi_stream_cfg_update_info {
  uint32_t stream_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t output_format;
  uint32_t user_stream_id;
  uint32_t frame_id;
  enum msm_vfe_frame_skip_pattern skip_pattern;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_vfe_axi_plane_cfg plane_cfg[MAX_PLANES_PER_STREAM];
  struct msm_isp_sw_framskip sw_skip_info;
};
struct msm_vfe_axi_halt_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stop_camif;
  uint32_t overflow_detected;
  uint32_t blocking_halt;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_axi_reset_cmd {
  uint32_t blocking;
  uint32_t frame_id;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_axi_restart_cmd {
  uint32_t enable_camif;
};
struct msm_vfe_axi_stream_update_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t num_streams;
  enum msm_vfe_axi_stream_update_type update_type;
  struct msm_vfe_axi_stream_cfg_update_info update_info[MSM_ISP_STATS_MAX];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_smmu_attach_cmd {
  uint32_t security_mode;
  uint32_t iommu_attach_mode;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_stats_stream_request_cmd {
  uint32_t session_id;
  uint32_t stream_id;
  enum msm_isp_stats_type stats_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t composite_flag;
  uint32_t framedrop_pattern;
  uint32_t init_frame_drop;
  uint32_t irq_subsample_pattern;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t buffer_offset;
  uint32_t stream_handle;
};
struct msm_vfe_stats_stream_release_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_handle;
};
struct msm_vfe_stats_stream_cfg_cmd {
  uint8_t num_streams;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_handle[MSM_ISP_STATS_MAX];
  uint8_t enable;
  uint32_t stats_burst_len;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_reg_cfg_type {
  VFE_WRITE,
  VFE_WRITE_MB,
  VFE_READ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_CFG_MASK,
  VFE_WRITE_DMI_16BIT,
  VFE_WRITE_DMI_32BIT,
  VFE_WRITE_DMI_64BIT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_READ_DMI_16BIT,
  VFE_READ_DMI_32BIT,
  VFE_READ_DMI_64BIT,
  GET_MAX_CLK_RATE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  GET_CLK_RATES,
  GET_ISP_ID,
  VFE_HW_UPDATE_LOCK,
  VFE_HW_UPDATE_UNLOCK,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  SET_WM_UB_SIZE,
  SET_UB_POLICY,
};
struct msm_vfe_cfg_cmd2 {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint16_t num_cfg;
  uint16_t cmd_len;
  void __user * cfg_data;
  void __user * cfg_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_cfg_cmd_list {
  struct msm_vfe_cfg_cmd2 cfg_cmd;
  struct msm_vfe_cfg_cmd_list * next;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t next_size;
};
struct msm_vfe_reg_rw_info {
  uint32_t reg_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t cmd_data_offset;
  uint32_t len;
};
struct msm_vfe_reg_mask_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t reg_offset;
  uint32_t mask;
  uint32_t val;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_reg_dmi_info {
  uint32_t hi_tbl_offset;
  uint32_t lo_tbl_offset;
  uint32_t len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_reg_cfg_cmd {
  union {
    struct msm_vfe_reg_rw_info rw_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_vfe_reg_mask_info mask_info;
    struct msm_vfe_reg_dmi_info dmi_info;
  } u;
  enum msm_vfe_reg_cfg_type cmd_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_isp_buf_type {
  ISP_PRIVATE_BUF,
  ISP_SHARE_BUF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MAX_ISP_BUF_TYPE,
};
struct msm_isp_buf_request {
  uint32_t session_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_id;
  uint8_t num_buf;
  uint32_t handle;
  enum msm_isp_buf_type buf_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_qbuf_plane {
  uint32_t addr;
  uint32_t offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t length;
};
struct msm_isp_qbuf_buffer {
  struct msm_isp_qbuf_plane planes[MAX_PLANES_PER_STREAM];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t num_planes;
};
struct msm_isp_qbuf_info {
  uint32_t handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t buf_idx;
  struct msm_isp_qbuf_buffer buffer;
  uint32_t dirty_buf;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_isp_clk_rates {
  uint32_t nominal_rate;
  uint32_t high_rate;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_axi_src_state {
  enum msm_vfe_input_src input_src;
  uint32_t src_active;
  uint32_t src_frame_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_isp_event_idx {
  ISP_REG_UPDATE = 0,
  ISP_EPOCH_0 = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_EPOCH_1 = 2,
  ISP_START_ACK = 3,
  ISP_STOP_ACK = 4,
  ISP_IRQ_VIOLATION = 5,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_STATS_OVERFLOW = 6,
  ISP_BUF_DONE = 7,
  ISP_FE_RD_DONE = 8,
  ISP_IOMMU_P_FAULT = 9,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_ERROR = 10,
  ISP_EVENT_MAX = 11
};
#define ISP_EVENT_OFFSET 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_BASE (V4L2_EVENT_PRIVATE_START)
#define ISP_BUF_EVENT_BASE (ISP_EVENT_BASE + (1 << ISP_EVENT_OFFSET))
#define ISP_STATS_EVENT_BASE (ISP_EVENT_BASE + (2 << ISP_EVENT_OFFSET))
#define ISP_CAMIF_EVENT_BASE (ISP_EVENT_BASE + (3 << ISP_EVENT_OFFSET))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_STREAM_EVENT_BASE (ISP_EVENT_BASE + (4 << ISP_EVENT_OFFSET))
#define ISP_EVENT_REG_UPDATE (ISP_EVENT_BASE + ISP_REG_UPDATE)
#define ISP_EVENT_EPOCH_0 (ISP_EVENT_BASE + ISP_EPOCH_0)
#define ISP_EVENT_EPOCH_1 (ISP_EVENT_BASE + ISP_EPOCH_1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_START_ACK (ISP_EVENT_BASE + ISP_START_ACK)
#define ISP_EVENT_STOP_ACK (ISP_EVENT_BASE + ISP_STOP_ACK)
#define ISP_EVENT_IRQ_VIOLATION (ISP_EVENT_BASE + ISP_IRQ_VIOLATION)
#define ISP_EVENT_STATS_OVERFLOW (ISP_EVENT_BASE + ISP_STATS_OVERFLOW)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_ERROR (ISP_EVENT_BASE + ISP_ERROR)
#define ISP_EVENT_SOF (ISP_CAMIF_EVENT_BASE)
#define ISP_EVENT_EOF (ISP_CAMIF_EVENT_BASE + 1)
#define ISP_EVENT_BUF_DONE (ISP_EVENT_BASE + ISP_BUF_DONE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_BUF_DIVERT (ISP_BUF_EVENT_BASE)
#define ISP_EVENT_STATS_NOTIFY (ISP_STATS_EVENT_BASE)
#define ISP_EVENT_COMP_STATS_NOTIFY (ISP_EVENT_STATS_NOTIFY + MSM_ISP_STATS_MAX)
#define ISP_EVENT_FE_READ_DONE (ISP_EVENT_BASE + ISP_FE_RD_DONE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_IOMMU_P_FAULT (ISP_EVENT_BASE + ISP_IOMMU_P_FAULT)
#define ISP_EVENT_STREAM_UPDATE_DONE (ISP_STREAM_EVENT_BASE)
struct msm_isp_buf_event {
  uint32_t session_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_id;
  uint32_t handle;
  uint32_t output_format;
  int8_t buf_idx;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_stats_event {
  uint32_t stats_mask;
  uint8_t stats_buf_idxs[MSM_ISP_STATS_MAX];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_stream_ack {
  uint32_t session_id;
  uint32_t stream_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t handle;
};
enum msm_vfe_error_type {
  ISP_ERROR_NONE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_ERROR_CAMIF,
  ISP_ERROR_BUS_OVERFLOW,
  ISP_ERROR_RETURN_EMPTY_BUFFER,
  ISP_ERROR_FRAME_ID_MISMATCH,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_ERROR_MAX,
};
struct msm_isp_error_info {
  enum msm_vfe_error_type err_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
};
struct msm_isp_output_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t regs_not_updated;
  uint32_t output_err_mask;
  uint16_t stream_framedrop_mask;
  uint32_t stats_framedrop_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_event_data {
  struct timeval timestamp;
  struct timeval mono_timestamp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t frame_id;
  union {
    struct msm_isp_stats_event stats;
    struct msm_isp_buf_event buf_done;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_isp_error_info error_info;
    struct msm_isp_output_info output_info;
  } u;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QBGGR8 v4l2_fourcc('Q', 'B', 'G', '8')
#define V4L2_PIX_FMT_QGBRG8 v4l2_fourcc('Q', 'G', 'B', '8')
#define V4L2_PIX_FMT_QGRBG8 v4l2_fourcc('Q', 'G', 'R', '8')
#define V4L2_PIX_FMT_QRGGB8 v4l2_fourcc('Q', 'R', 'G', '8')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QBGGR10 v4l2_fourcc('Q', 'B', 'G', '0')
#define V4L2_PIX_FMT_QGBRG10 v4l2_fourcc('Q', 'G', 'B', '0')
#define V4L2_PIX_FMT_QGRBG10 v4l2_fourcc('Q', 'G', 'R', '0')
#define V4L2_PIX_FMT_QRGGB10 v4l2_fourcc('Q', 'R', 'G', '0')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QBGGR12 v4l2_fourcc('Q', 'B', 'G', '2')
#define V4L2_PIX_FMT_QGBRG12 v4l2_fourcc('Q', 'G', 'B', '2')
#define V4L2_PIX_FMT_QGRBG12 v4l2_fourcc('Q', 'G', 'R', '2')
#define V4L2_PIX_FMT_QRGGB12 v4l2_fourcc('Q', 'R', 'G', '2')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QBGGR14 v4l2_fourcc('Q', 'B', 'G', '4')
#define V4L2_PIX_FMT_QGBRG14 v4l2_fourcc('Q', 'G', 'B', '4')
#define V4L2_PIX_FMT_QGRBG14 v4l2_fourcc('Q', 'G', 'R', '4')
#define V4L2_PIX_FMT_QRGGB14 v4l2_fourcc('Q', 'R', 'G', '4')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_P16BGGR10 v4l2_fourcc('P', 'B', 'G', '0')
#define V4L2_PIX_FMT_P16GBRG10 v4l2_fourcc('P', 'G', 'B', '0')
#define V4L2_PIX_FMT_P16GRBG10 v4l2_fourcc('P', 'G', 'R', '0')
#define V4L2_PIX_FMT_P16RGGB10 v4l2_fourcc('P', 'R', 'G', '0')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_NV14 v4l2_fourcc('N', 'V', '1', '4')
#define V4L2_PIX_FMT_NV41 v4l2_fourcc('N', 'V', '4', '1')
#define V4L2_PIX_FMT_META v4l2_fourcc('Q', 'M', 'E', 'T')
#define V4L2_PIX_FMT_SBGGR14 v4l2_fourcc('B', 'G', '1', '4')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_SGBRG14 v4l2_fourcc('G', 'B', '1', '4')
#define V4L2_PIX_FMT_SGRBG14 v4l2_fourcc('B', 'A', '1', '4')
#define V4L2_PIX_FMT_SRGGB14 v4l2_fourcc('R', 'G', '1', '4')
#define VIDIOC_MSM_VFE_REG_CFG _IOWR('V', BASE_VIDIOC_PRIVATE, struct msm_vfe_cfg_cmd2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_REQUEST_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 1, struct msm_isp_buf_request)
#define VIDIOC_MSM_ISP_ENQUEUE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 2, struct msm_isp_qbuf_info)
#define VIDIOC_MSM_ISP_RELEASE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 3, struct msm_isp_buf_request)
#define VIDIOC_MSM_ISP_REQUEST_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 4, struct msm_vfe_axi_stream_request_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_CFG_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 5, struct msm_vfe_axi_stream_cfg_cmd)
#define VIDIOC_MSM_ISP_RELEASE_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct msm_vfe_axi_stream_release_cmd)
#define VIDIOC_MSM_ISP_INPUT_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 7, struct msm_vfe_input_cfg)
#define VIDIOC_MSM_ISP_SET_SRC_STATE _IOWR('V', BASE_VIDIOC_PRIVATE + 8, struct msm_vfe_axi_src_state)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_REQUEST_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 9, struct msm_vfe_stats_stream_request_cmd)
#define VIDIOC_MSM_ISP_CFG_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 10, struct msm_vfe_stats_stream_cfg_cmd)
#define VIDIOC_MSM_ISP_RELEASE_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 11, struct msm_vfe_stats_stream_release_cmd)
#define VIDIOC_MSM_ISP_REG_UPDATE_CMD _IOWR('V', BASE_VIDIOC_PRIVATE + 12, enum msm_vfe_input_src)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_UPDATE_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 13, struct msm_vfe_axi_stream_update_cmd)
#define VIDIOC_MSM_VFE_REG_LIST_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 14, struct msm_vfe_cfg_cmd_list)
#define VIDIOC_MSM_ISP_SMMU_ATTACH _IOWR('V', BASE_VIDIOC_PRIVATE + 15, struct msm_vfe_smmu_attach_cmd)
#define VIDIOC_MSM_ISP_UPDATE_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 16, struct msm_vfe_axi_stream_update_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_AXI_HALT _IOWR('V', BASE_VIDIOC_PRIVATE + 17, struct msm_vfe_axi_halt_cmd)
#define VIDIOC_MSM_ISP_AXI_RESET _IOWR('V', BASE_VIDIOC_PRIVATE + 18, struct msm_vfe_axi_reset_cmd)
#define VIDIOC_MSM_ISP_AXI_RESTART _IOWR('V', BASE_VIDIOC_PRIVATE + 19, struct msm_vfe_axi_restart_cmd)
#define VIDIOC_MSM_ISP_FETCH_ENG_START _IOWR('V', BASE_VIDIOC_PRIVATE + 20, struct msm_vfe_fetch_eng_start)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_DEQUEUE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 21, struct msm_isp_qbuf_info)
#endif
