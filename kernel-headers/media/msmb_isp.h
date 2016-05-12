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
#define ISP_OFFLINE_STATS_BIT (0x10000 << 5)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_STATS_STREAM_BIT 0x80000000
struct msm_vfe_cfg_cmd_list;
enum ISP_START_PIXEL_PATTERN {
  ISP_BAYER_RGRGRG,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_BAYER_GRGRGR,
  ISP_BAYER_BGBGBG,
  ISP_BAYER_GBGBGB,
  ISP_YUV_YCbYCr,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_YUV_YCrYCb,
  ISP_YUV_CbYCrY,
  ISP_YUV_CrYCbY,
  ISP_PIX_PATTERN_MAX
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_plane_fmt {
  Y_PLANE,
  CB_PLANE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CR_PLANE,
  CRCB_PLANE,
  CBCR_PLANE,
  VFE_PLANE_FMT_MAX
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_input_src {
  VFE_PIX_0,
  VFE_RAW_0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_RAW_1,
  VFE_RAW_2,
  VFE_SRC_MAX,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_axi_stream_src {
  PIX_ENCODER,
  PIX_VIEWFINDER,
  PIX_VIDEO,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CAMIF_RAW,
  IDEAL_RAW,
  RDI_INTF_0,
  RDI_INTF_1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  RDI_INTF_2,
  VFE_AXI_SRC_MAX
};
enum msm_vfe_frame_skip_pattern {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  NO_SKIP,
  EVERY_2FRAME,
  EVERY_3FRAME,
  EVERY_4FRAME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  EVERY_5FRAME,
  EVERY_6FRAME,
  EVERY_7FRAME,
  EVERY_8FRAME,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  EVERY_16FRAME,
  EVERY_32FRAME,
  SKIP_ALL,
  SKIP_RANGE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MAX_SKIP,
};
enum msm_isp_stats_type {
  MSM_ISP_STATS_AEC,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_AF,
  MSM_ISP_STATS_AWB,
  MSM_ISP_STATS_RS,
  MSM_ISP_STATS_CS,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_IHIST,
  MSM_ISP_STATS_SKIN,
  MSM_ISP_STATS_BG,
  MSM_ISP_STATS_BF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_BE,
  MSM_ISP_STATS_BHIST,
  MSM_ISP_STATS_BF_SCALE,
  MSM_ISP_STATS_HDR_BE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_ISP_STATS_HDR_BHIST,
  MSM_ISP_STATS_AEC_BG,
  MSM_ISP_STATS_MAX
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_isp_sw_framskip {
  uint32_t stats_type_mask;
  uint32_t stream_src_mask;
  enum msm_vfe_frame_skip_pattern skip_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t min_frame_id;
  uint32_t max_frame_id;
};
enum msm_vfe_testgen_color_pattern {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  COLOR_BAR_8_COLOR,
  UNICOLOR_WHITE,
  UNICOLOR_YELLOW,
  UNICOLOR_CYAN,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UNICOLOR_GREEN,
  UNICOLOR_MAGENTA,
  UNICOLOR_RED,
  UNICOLOR_BLUE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UNICOLOR_BLACK,
  MAX_COLOR,
};
enum msm_vfe_camif_input {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  CAMIF_DISABLED,
  CAMIF_PAD_REG_INPUT,
  CAMIF_MIDDI_INPUT,
  CAMIF_MIPI_INPUT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_fetch_engine_cfg {
  uint32_t input_format;
  uint32_t buf_width;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t buf_height;
  uint32_t fetch_width;
  uint32_t fetch_height;
  uint32_t x_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t y_offset;
  uint32_t buf_stride;
};
struct msm_vfe_camif_subsample_cfg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t irq_subsample_period;
  uint32_t irq_subsample_pattern;
  uint32_t sof_counter_step;
  uint32_t pixel_skip;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t line_skip;
};
struct msm_vfe_camif_cfg {
  uint32_t lines_per_frame;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t pixels_per_line;
  uint32_t first_pixel;
  uint32_t last_pixel;
  uint32_t first_line;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t last_line;
  uint32_t epoch_line0;
  uint32_t epoch_line1;
  enum msm_vfe_camif_input camif_input;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_vfe_camif_subsample_cfg subsample_cfg;
};
struct msm_vfe_testgen_cfg {
  uint32_t lines_per_frame;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t pixels_per_line;
  uint32_t v_blank;
  uint32_t h_blank;
  enum ISP_START_PIXEL_PATTERN pixel_bayer_pattern;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t rotate_period;
  enum msm_vfe_testgen_color_pattern color_bar_pattern;
  uint32_t burst_num_frame;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_inputmux {
  CAMIF,
  TESTGEN,
  EXTERNAL_READ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_stats_composite_group {
  STATS_COMPOSITE_GRP_NONE,
  STATS_COMPOSITE_GRP_1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  STATS_COMPOSITE_GRP_2,
  STATS_COMPOSITE_GRP_MAX,
};
enum msm_vfe_hvx_streaming_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  HVX_DISABLE,
  HVX_ONE_WAY,
  HVX_ROUND_TRIP
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_pix_cfg {
  struct msm_vfe_camif_cfg camif_cfg;
  struct msm_vfe_testgen_cfg testgen_cfg;
  struct msm_vfe_fetch_engine_cfg fetch_engine_cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_inputmux input_mux;
  enum ISP_START_PIXEL_PATTERN pixel_pattern;
  uint32_t input_format;
  enum msm_vfe_hvx_streaming_cmd hvx_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t is_split;
};
struct msm_vfe_rdi_cfg {
  uint8_t cid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t frame_based;
};
struct msm_vfe_input_cfg {
  union {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_vfe_pix_cfg pix_cfg;
    struct msm_vfe_rdi_cfg rdi_cfg;
  } d;
  enum msm_vfe_input_src input_src;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t input_pix_clk;
};
struct msm_vfe_fetch_eng_start {
  uint32_t session_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_id;
  uint32_t buf_idx;
  uint8_t offline_mode;
  uint32_t fd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t buf_addr;
  uint32_t frame_id;
};
struct msm_vfe_axi_plane_cfg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t output_width;
  uint32_t output_height;
  uint32_t output_stride;
  uint32_t output_scan_lines;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t output_plane_format;
  uint32_t plane_addr_offset;
  uint8_t csid_src;
  uint8_t rdi_cid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_stream_memory_input_t {
  MEMORY_INPUT_DISABLED,
  MEMORY_INPUT_ENABLED
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_axi_stream_request_cmd {
  uint32_t session_id;
  uint32_t stream_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t vt_enable;
  uint32_t output_format;
  enum msm_vfe_axi_stream_src stream_src;
  struct msm_vfe_axi_plane_cfg plane_cfg[MAX_PLANES_PER_STREAM];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t burst_count;
  uint32_t hfr_mode;
  uint8_t frame_base;
  uint32_t init_frame_drop;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_frame_skip_pattern frame_skip_pattern;
  uint8_t buf_divert;
  uint32_t axi_stream_handle;
  uint32_t controllable_output;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t burst_len;
  enum msm_stream_memory_input_t memory_input;
};
struct msm_vfe_axi_stream_release_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_handle;
};
enum msm_vfe_axi_stream_cmd {
  STOP_STREAM,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  START_STREAM,
  STOP_IMMEDIATELY,
};
struct msm_vfe_axi_stream_cfg_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t num_streams;
  uint32_t stream_handle[VFE_AXI_SRC_MAX];
  enum msm_vfe_axi_stream_cmd cmd;
  uint8_t sync_frame_id_src;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vfe_axi_stream_update_type {
  ENABLE_STREAM_BUF_DIVERT,
  DISABLE_STREAM_BUF_DIVERT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UPDATE_STREAM_FRAMEDROP_PATTERN,
  UPDATE_STREAM_STATS_FRAMEDROP_PATTERN,
  UPDATE_STREAM_AXI_CONFIG,
  UPDATE_STREAM_REQUEST_FRAMES,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  UPDATE_STREAM_ADD_BUFQ,
  UPDATE_STREAM_REMOVE_BUFQ,
  UPDATE_STREAM_SW_FRAME_DROP,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_iommu_type {
  IOMMU_ATTACH,
  IOMMU_DETACH,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_buff_queue_id {
  VFE_BUF_QUEUE_DEFAULT,
  VFE_BUF_QUEUE_SHARED,
  VFE_BUF_QUEUE_MAX,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_axi_stream_cfg_update_info {
  uint32_t stream_handle;
  uint32_t output_format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t user_stream_id;
  uint32_t frame_id;
  enum msm_vfe_frame_skip_pattern skip_pattern;
  struct msm_vfe_axi_plane_cfg plane_cfg[MAX_PLANES_PER_STREAM];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_isp_sw_framskip sw_skip_info;
};
struct msm_vfe_axi_halt_cmd {
  uint32_t stop_camif;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t overflow_detected;
  uint32_t blocking_halt;
};
struct msm_vfe_axi_reset_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t blocking;
  uint32_t frame_id;
};
struct msm_vfe_axi_restart_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t enable_camif;
};
struct msm_vfe_axi_stream_update_cmd {
  uint32_t num_streams;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_axi_stream_update_type update_type;
  struct msm_vfe_axi_stream_cfg_update_info update_info[MSM_ISP_STATS_MAX];
};
struct msm_vfe_smmu_attach_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t security_mode;
  uint32_t iommu_attach_mode;
};
struct msm_vfe_stats_stream_request_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
  enum msm_isp_stats_type stats_type;
  uint32_t composite_flag;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t framedrop_pattern;
  uint32_t init_frame_drop;
  uint32_t irq_subsample_pattern;
  uint32_t buffer_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_handle;
};
struct msm_vfe_stats_stream_release_cmd {
  uint32_t stream_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_stats_stream_cfg_cmd {
  uint8_t num_streams;
  uint32_t stream_handle[MSM_ISP_STATS_MAX];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t enable;
  uint32_t stats_burst_len;
};
enum msm_vfe_reg_cfg_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_WRITE,
  VFE_WRITE_MB,
  VFE_READ,
  VFE_CFG_MASK,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_WRITE_DMI_16BIT,
  VFE_WRITE_DMI_32BIT,
  VFE_WRITE_DMI_64BIT,
  VFE_READ_DMI_16BIT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_READ_DMI_32BIT,
  VFE_READ_DMI_64BIT,
  GET_MAX_CLK_RATE,
  GET_CLK_RATES,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  GET_ISP_ID,
  VFE_HW_UPDATE_LOCK,
  VFE_HW_UPDATE_UNLOCK,
  SET_WM_UB_SIZE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  SET_UB_POLICY,
};
struct msm_vfe_cfg_cmd2 {
  uint16_t num_cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint16_t cmd_len;
  void __user * cfg_data;
  void __user * cfg_cmd;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_cfg_cmd_list {
  struct msm_vfe_cfg_cmd2 cfg_cmd;
  struct msm_vfe_cfg_cmd_list * next;
  uint32_t next_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vfe_reg_rw_info {
  uint32_t reg_offset;
  uint32_t cmd_data_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t len;
};
struct msm_vfe_reg_mask_info {
  uint32_t reg_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t mask;
  uint32_t val;
};
struct msm_vfe_reg_dmi_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t hi_tbl_offset;
  uint32_t lo_tbl_offset;
  uint32_t len;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vfe_reg_cfg_cmd {
  union {
    struct msm_vfe_reg_rw_info rw_info;
    struct msm_vfe_reg_mask_info mask_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_vfe_reg_dmi_info dmi_info;
  } u;
  enum msm_vfe_reg_cfg_type cmd_type;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vfe_sd_type {
  VFE_SD_0 = 0,
  VFE_SD_1,
  VFE_SD_COMMON,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VFE_SD_MAX,
};
#define MS_NUM_SLAVE_MAX 1
enum msm_vfe_dual_hw_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  DUAL_NONE = 0,
  DUAL_HW_VFE_SPLIT = 1,
  DUAL_HW_MASTER_SLAVE = 2,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vfe_dual_hw_ms_type {
  MS_TYPE_NONE,
  MS_TYPE_MASTER,
  MS_TYPE_SLAVE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_set_dual_hw_ms_cmd {
  uint8_t num_src;
  enum msm_vfe_dual_hw_ms_type dual_hw_ms_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_input_src primary_intf;
  enum msm_vfe_input_src input_src[VFE_SRC_MAX];
  uint32_t sof_delta_threshold;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_isp_buf_type {
  ISP_PRIVATE_BUF,
  ISP_SHARE_BUF,
  MAX_ISP_BUF_TYPE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_unmap_buf_req {
  uint32_t fd;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_isp_buf_request {
  uint32_t session_id;
  uint32_t stream_id;
  uint8_t num_buf;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t handle;
  enum msm_isp_buf_type buf_type;
};
struct msm_isp_qbuf_plane {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t addr;
  uint32_t offset;
  uint32_t length;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_isp_qbuf_buffer {
  struct msm_isp_qbuf_plane planes[MAX_PLANES_PER_STREAM];
  uint32_t num_planes;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_isp_qbuf_info {
  uint32_t handle;
  int32_t buf_idx;
  struct msm_isp_qbuf_buffer buffer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t dirty_buf;
};
struct msm_isp_clk_rates {
  uint32_t svs_rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t nominal_rate;
  uint32_t high_rate;
};
struct msm_vfe_axi_src_state {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_input_src input_src;
  uint32_t src_active;
  uint32_t src_frame_id;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_isp_event_mask_index {
  ISP_EVENT_MASK_INDEX_STATS_NOTIFY = 0,
  ISP_EVENT_MASK_INDEX_ERROR = 1,
  ISP_EVENT_MASK_INDEX_IOMMU_P_FAULT = 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_EVENT_MASK_INDEX_STREAM_UPDATE_DONE = 3,
  ISP_EVENT_MASK_INDEX_REG_UPDATE = 4,
  ISP_EVENT_MASK_INDEX_SOF = 5,
  ISP_EVENT_MASK_INDEX_BUF_DIVERT = 6,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_EVENT_MASK_INDEX_COMP_STATS_NOTIFY = 7,
  ISP_EVENT_MASK_INDEX_MASK_FE_READ_DONE = 8,
  ISP_EVENT_MASK_INDEX_HW_FATAL_ERROR = 9,
  ISP_EVENT_MASK_INDEX_PING_PONG_MISMATCH = 10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_EVENT_MASK_INDEX_REG_UPDATE_MISSING = 11,
  ISP_EVENT_MASK_INDEX_BUF_FATAL_ERROR = 12,
  ISP_EVENT_MASK_INDEX_MAX = 13
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_SUBS_MASK_NONE 0
#define ISP_EVENT_SUBS_MASK_STATS_NOTIFY (1 << ISP_EVENT_MASK_INDEX_STATS_NOTIFY)
#define ISP_EVENT_SUBS_MASK_ERROR (1 << ISP_EVENT_MASK_INDEX_ERROR)
#define ISP_EVENT_SUBS_MASK_IOMMU_P_FAULT (1 << ISP_EVENT_MASK_INDEX_IOMMU_P_FAULT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_SUBS_MASK_STREAM_UPDATE_DONE (1 << ISP_EVENT_MASK_INDEX_STREAM_UPDATE_DONE)
#define ISP_EVENT_SUBS_MASK_REG_UPDATE (1 << ISP_EVENT_MASK_INDEX_REG_UPDATE)
#define ISP_EVENT_SUBS_MASK_SOF (1 << ISP_EVENT_MASK_INDEX_SOF)
#define ISP_EVENT_SUBS_MASK_BUF_DIVERT (1 << ISP_EVENT_MASK_INDEX_BUF_DIVERT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_SUBS_MASK_COMP_STATS_NOTIFY (1 << ISP_EVENT_MASK_INDEX_COMP_STATS_NOTIFY)
#define ISP_EVENT_SUBS_MASK_FE_READ_DONE (1 << ISP_EVENT_MASK_INDEX_MASK_FE_READ_DONE)
#define ISP_EVENT_SUBS_MASK_HW_FATAL_ERROR (1 << ISP_EVENT_MASK_INDEX_HW_FATAL_ERROR)
#define ISP_EVENT_SUBS_MASK_PING_PONG_MISMATCH (1 << ISP_EVENT_MASK_INDEX_PING_PONG_MISMATCH)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_SUBS_MASK_REG_UPDATE_MISSING (1 << ISP_EVENT_MASK_INDEX_REG_UPDATE_MISSING)
#define ISP_EVENT_SUBS_MASK_BUF_FATAL_ERROR (1 << ISP_EVENT_MASK_INDEX_BUF_FATAL_ERROR)
enum msm_isp_event_idx {
  ISP_REG_UPDATE = 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_EPOCH_0 = 1,
  ISP_EPOCH_1 = 2,
  ISP_START_ACK = 3,
  ISP_STOP_ACK = 4,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_IRQ_VIOLATION = 5,
  ISP_STATS_OVERFLOW = 6,
  ISP_BUF_DONE = 7,
  ISP_FE_RD_DONE = 8,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_IOMMU_P_FAULT = 9,
  ISP_ERROR = 10,
  ISP_HW_FATAL_ERROR = 11,
  ISP_PING_PONG_MISMATCH = 12,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_REG_UPDATE_MISSING = 13,
  ISP_BUF_FATAL_ERROR = 14,
  ISP_EVENT_MAX = 15
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_OFFSET 8
#define ISP_EVENT_BASE (V4L2_EVENT_PRIVATE_START)
#define ISP_BUF_EVENT_BASE (ISP_EVENT_BASE + (1 << ISP_EVENT_OFFSET))
#define ISP_STATS_EVENT_BASE (ISP_EVENT_BASE + (2 << ISP_EVENT_OFFSET))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_CAMIF_EVENT_BASE (ISP_EVENT_BASE + (3 << ISP_EVENT_OFFSET))
#define ISP_STREAM_EVENT_BASE (ISP_EVENT_BASE + (4 << ISP_EVENT_OFFSET))
#define ISP_EVENT_REG_UPDATE (ISP_EVENT_BASE + ISP_REG_UPDATE)
#define ISP_EVENT_EPOCH_0 (ISP_EVENT_BASE + ISP_EPOCH_0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_EPOCH_1 (ISP_EVENT_BASE + ISP_EPOCH_1)
#define ISP_EVENT_START_ACK (ISP_EVENT_BASE + ISP_START_ACK)
#define ISP_EVENT_STOP_ACK (ISP_EVENT_BASE + ISP_STOP_ACK)
#define ISP_EVENT_IRQ_VIOLATION (ISP_EVENT_BASE + ISP_IRQ_VIOLATION)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_STATS_OVERFLOW (ISP_EVENT_BASE + ISP_STATS_OVERFLOW)
#define ISP_EVENT_ERROR (ISP_EVENT_BASE + ISP_ERROR)
#define ISP_EVENT_SOF (ISP_CAMIF_EVENT_BASE)
#define ISP_EVENT_EOF (ISP_CAMIF_EVENT_BASE + 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_BUF_DONE (ISP_EVENT_BASE + ISP_BUF_DONE)
#define ISP_EVENT_BUF_DIVERT (ISP_BUF_EVENT_BASE)
#define ISP_EVENT_STATS_NOTIFY (ISP_STATS_EVENT_BASE)
#define ISP_EVENT_COMP_STATS_NOTIFY (ISP_EVENT_STATS_NOTIFY + MSM_ISP_STATS_MAX)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_FE_READ_DONE (ISP_EVENT_BASE + ISP_FE_RD_DONE)
#define ISP_EVENT_IOMMU_P_FAULT (ISP_EVENT_BASE + ISP_IOMMU_P_FAULT)
#define ISP_EVENT_HW_FATAL_ERROR (ISP_EVENT_BASE + ISP_HW_FATAL_ERROR)
#define ISP_EVENT_PING_PONG_MISMATCH (ISP_EVENT_BASE + ISP_PING_PONG_MISMATCH)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ISP_EVENT_REG_UPDATE_MISSING (ISP_EVENT_BASE + ISP_REG_UPDATE_MISSING)
#define ISP_EVENT_BUF_FATAL_ERROR (ISP_EVENT_BASE + ISP_BUF_FATAL_ERROR)
#define ISP_EVENT_STREAM_UPDATE_DONE (ISP_STREAM_EVENT_BASE)
struct msm_isp_buf_event {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
  uint32_t handle;
  uint32_t output_format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int8_t buf_idx;
};
struct msm_isp_fetch_eng_event {
  uint32_t session_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_id;
  uint32_t handle;
  uint32_t fd;
  int8_t buf_idx;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int8_t offline_mode;
};
struct msm_isp_stats_event {
  uint32_t stats_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t stats_buf_idxs[MSM_ISP_STATS_MAX];
};
struct msm_isp_stream_ack {
  uint32_t session_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stream_id;
  uint32_t handle;
};
enum msm_vfe_error_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_ERROR_NONE,
  ISP_ERROR_CAMIF,
  ISP_ERROR_BUS_OVERFLOW,
  ISP_ERROR_RETURN_EMPTY_BUFFER,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ISP_ERROR_FRAME_ID_MISMATCH,
  ISP_ERROR_MAX,
};
struct msm_isp_error_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_vfe_error_type err_type;
  uint32_t session_id;
  uint32_t stream_id;
  uint8_t stream_id_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_ms_delta_info {
  uint8_t num_delta_info;
  uint32_t delta[MS_NUM_SLAVE_MAX];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_isp_output_info {
  uint8_t regs_not_updated;
  uint16_t output_err_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t stream_framedrop_mask;
  uint16_t stats_framedrop_mask;
};
struct msm_isp_sof_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t regs_not_updated;
  uint16_t reg_update_fail_mask;
  uint32_t stream_get_buf_fail_mask;
  uint16_t stats_get_buf_fail_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_isp_ms_delta_info ms_delta_info;
};
struct msm_isp_event_data {
  struct timeval timestamp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct timeval mono_timestamp;
  uint32_t frame_id;
  union {
    struct msm_isp_stats_event stats;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_isp_buf_event buf_done;
    struct msm_isp_fetch_eng_event fetch_done;
    struct msm_isp_error_info error_info;
    struct msm_isp_output_info output_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_isp_sof_info sof_info;
  } u;
};
struct msm_isp_set_stats_ab {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint64_t stats_ab;
};
#define V4L2_PIX_FMT_QBGGR8 v4l2_fourcc('Q', 'B', 'G', '8')
#define V4L2_PIX_FMT_QGBRG8 v4l2_fourcc('Q', 'G', 'B', '8')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QGRBG8 v4l2_fourcc('Q', 'G', 'R', '8')
#define V4L2_PIX_FMT_QRGGB8 v4l2_fourcc('Q', 'R', 'G', '8')
#define V4L2_PIX_FMT_QBGGR10 v4l2_fourcc('Q', 'B', 'G', '0')
#define V4L2_PIX_FMT_QGBRG10 v4l2_fourcc('Q', 'G', 'B', '0')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QGRBG10 v4l2_fourcc('Q', 'G', 'R', '0')
#define V4L2_PIX_FMT_QRGGB10 v4l2_fourcc('Q', 'R', 'G', '0')
#define V4L2_PIX_FMT_QBGGR12 v4l2_fourcc('Q', 'B', 'G', '2')
#define V4L2_PIX_FMT_QGBRG12 v4l2_fourcc('Q', 'G', 'B', '2')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QGRBG12 v4l2_fourcc('Q', 'G', 'R', '2')
#define V4L2_PIX_FMT_QRGGB12 v4l2_fourcc('Q', 'R', 'G', '2')
#define V4L2_PIX_FMT_QBGGR14 v4l2_fourcc('Q', 'B', 'G', '4')
#define V4L2_PIX_FMT_QGBRG14 v4l2_fourcc('Q', 'G', 'B', '4')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_QGRBG14 v4l2_fourcc('Q', 'G', 'R', '4')
#define V4L2_PIX_FMT_QRGGB14 v4l2_fourcc('Q', 'R', 'G', '4')
#define V4L2_PIX_FMT_P16BGGR10 v4l2_fourcc('P', 'B', 'G', '0')
#define V4L2_PIX_FMT_P16GBRG10 v4l2_fourcc('P', 'G', 'B', '0')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_P16GRBG10 v4l2_fourcc('P', 'G', 'R', '0')
#define V4L2_PIX_FMT_P16RGGB10 v4l2_fourcc('P', 'R', 'G', '0')
#define V4L2_PIX_FMT_NV14 v4l2_fourcc('N', 'V', '1', '4')
#define V4L2_PIX_FMT_NV41 v4l2_fourcc('N', 'V', '4', '1')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_META v4l2_fourcc('Q', 'M', 'E', 'T')
#define V4L2_PIX_FMT_SBGGR14 v4l2_fourcc('B', 'G', '1', '4')
#define V4L2_PIX_FMT_SGBRG14 v4l2_fourcc('G', 'B', '1', '4')
#define V4L2_PIX_FMT_SGRBG14 v4l2_fourcc('B', 'A', '1', '4')
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_PIX_FMT_SRGGB14 v4l2_fourcc('R', 'G', '1', '4')
#define VIDIOC_MSM_VFE_REG_CFG _IOWR('V', BASE_VIDIOC_PRIVATE, struct msm_vfe_cfg_cmd2)
#define VIDIOC_MSM_ISP_REQUEST_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 1, struct msm_isp_buf_request)
#define VIDIOC_MSM_ISP_ENQUEUE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 2, struct msm_isp_qbuf_info)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_RELEASE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 3, struct msm_isp_buf_request)
#define VIDIOC_MSM_ISP_REQUEST_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 4, struct msm_vfe_axi_stream_request_cmd)
#define VIDIOC_MSM_ISP_CFG_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 5, struct msm_vfe_axi_stream_cfg_cmd)
#define VIDIOC_MSM_ISP_RELEASE_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct msm_vfe_axi_stream_release_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_INPUT_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 7, struct msm_vfe_input_cfg)
#define VIDIOC_MSM_ISP_SET_SRC_STATE _IOWR('V', BASE_VIDIOC_PRIVATE + 8, struct msm_vfe_axi_src_state)
#define VIDIOC_MSM_ISP_REQUEST_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 9, struct msm_vfe_stats_stream_request_cmd)
#define VIDIOC_MSM_ISP_CFG_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 10, struct msm_vfe_stats_stream_cfg_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_RELEASE_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 11, struct msm_vfe_stats_stream_release_cmd)
#define VIDIOC_MSM_ISP_REG_UPDATE_CMD _IOWR('V', BASE_VIDIOC_PRIVATE + 12, enum msm_vfe_input_src)
#define VIDIOC_MSM_ISP_UPDATE_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 13, struct msm_vfe_axi_stream_update_cmd)
#define VIDIOC_MSM_VFE_REG_LIST_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 14, struct msm_vfe_cfg_cmd_list)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_SMMU_ATTACH _IOWR('V', BASE_VIDIOC_PRIVATE + 15, struct msm_vfe_smmu_attach_cmd)
#define VIDIOC_MSM_ISP_UPDATE_STATS_STREAM _IOWR('V', BASE_VIDIOC_PRIVATE + 16, struct msm_vfe_axi_stream_update_cmd)
#define VIDIOC_MSM_ISP_AXI_HALT _IOWR('V', BASE_VIDIOC_PRIVATE + 17, struct msm_vfe_axi_halt_cmd)
#define VIDIOC_MSM_ISP_AXI_RESET _IOWR('V', BASE_VIDIOC_PRIVATE + 18, struct msm_vfe_axi_reset_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_AXI_RESTART _IOWR('V', BASE_VIDIOC_PRIVATE + 19, struct msm_vfe_axi_restart_cmd)
#define VIDIOC_MSM_ISP_FETCH_ENG_START _IOWR('V', BASE_VIDIOC_PRIVATE + 20, struct msm_vfe_fetch_eng_start)
#define VIDIOC_MSM_ISP_DEQUEUE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 21, struct msm_isp_qbuf_info)
#define VIDIOC_MSM_ISP_SET_DUAL_HW_MASTER_SLAVE _IOWR('V', BASE_VIDIOC_PRIVATE + 22, struct msm_isp_set_dual_hw_ms_cmd)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_ISP_SET_STATS_BANDWIDTH _IOWR('V', BASE_VIDIOC_PRIVATE + 23, struct msm_isp_set_stats_ab)
#define VIDIOC_MSM_ISP_MAP_BUF_START_FE _IOWR('V', BASE_VIDIOC_PRIVATE + 24, struct msm_vfe_fetch_eng_start)
#define VIDIOC_MSM_ISP_UNMAP_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 25, struct msm_isp_unmap_buf_req)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
