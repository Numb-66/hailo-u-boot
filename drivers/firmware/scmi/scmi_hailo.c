#include <common.h>
#include <dm/device.h>
#include <scmi_agent.h>
#include <scmi_protocols.h>

#include <scmi_hailo.h>
#include <dt-bindings/soc/hailo15_release_version.h>

struct scmi_hailo_empty_in {};
struct scmi_hailo_empty_out {};

#define DECLARE_SCMI_HAILO_OUT(_struct_name, _out_variable_name) \
	struct { \
		int32_t status; \
		struct _struct_name response; \
	} __packed _out_variable_name

int scmi_hailo_configure_ethernet_delay(
		struct udevice *dev, uint8_t tx_bypass_clock_delay, uint8_t tx_clock_inversion,
		uint8_t tx_clock_delay, uint8_t rx_bypass_clock_delay,
		uint8_t rx_clock_inversion, uint8_t rx_clock_delay) {
	int ret;
	struct scmi_hailo_eth_delay_configuration_a2p in = {
			.tx_bypass_clock_delay = rx_bypass_clock_delay,
			.tx_clock_inversion = tx_clock_inversion,
			.tx_clock_delay = tx_clock_delay,
			.rx_bypass_clock_delay = rx_bypass_clock_delay,
			.rx_clock_inversion = rx_clock_inversion,
			.rx_clock_delay = rx_clock_delay,
	};
	DECLARE_SCMI_HAILO_OUT(scmi_hailo_empty_out, out);

	struct scmi_msg msg = SCMI_MSG_IN(SCMI_PROTOCOL_ID_HAILO,
					  SCMI_HAILO_CONFIGURE_ETH_DELAY_ID, in, out);

	ret = devm_scmi_process_msg(dev, &msg);
	if (ret)
		return ret;

	if (out.status)
		return scmi_to_linux_errno(out.status);

	return 0;
}

int scmi_hailo_set_eth_rmii(struct udevice *dev) {
	int ret;
	struct scmi_hailo_empty_in in = {};
	DECLARE_SCMI_HAILO_OUT(scmi_hailo_empty_out, out);
	struct scmi_msg msg = SCMI_MSG_IN(SCMI_PROTOCOL_ID_HAILO,
					  SCMI_HAILO_SET_ETH_RMII_MODE_ID, in, out);

	ret = devm_scmi_process_msg(dev, &msg);
	if (ret)
		return ret;

	if (out.status)
		return scmi_to_linux_errno(out.status);

	return 0;
}

int scmi_hailo_get_boot_info(struct udevice *dev, struct scmi_hailo_get_boot_info_p2a *boot_info) {
	int ret;
	struct scmi_hailo_empty_in in = {};
	DECLARE_SCMI_HAILO_OUT(scmi_hailo_get_boot_info_p2a, out);
	struct scmi_msg msg = SCMI_MSG_IN(SCMI_PROTOCOL_ID_HAILO,
					  SCMI_HAILO_GET_BOOT_INFO_ID, in, out);

	ret = devm_scmi_process_msg(dev, &msg);
	if (ret)
		return ret;

	if (out.status)
		return scmi_to_linux_errno(out.status);

	*boot_info = out.response;

	return 0;
}

int scmi_hailo_send_boot_success_ind(struct udevice *dev) {
	int ret;
	struct scmi_hailo_boot_success_indication_a2p in = {
			.component = SCMI_HAILO_BOOT_SUCCESS_COMPONENT_AP_BOOTLOADER,
			.component_version = HAILO_UBOOT_RELEASE_BUILD_VERSION,
	};
	DECLARE_SCMI_HAILO_OUT(scmi_hailo_empty_out, out);

	struct scmi_msg msg = SCMI_MSG_IN(SCMI_PROTOCOL_ID_HAILO,
					  SCMI_HAILO_BOOT_SUCCESS_INDICATION_ID, in, out);

	ret = devm_scmi_process_msg(dev, &msg);
	if (ret)
		return ret;

	if (out.status)
		return scmi_to_linux_errno(out.status);

	return 0;
}

int scmi_hailo_get_sku_id(struct udevice *dev, struct scmi_hailo_get_sku_id_p2a *sku_id) {
	int ret;
	struct scmi_hailo_empty_in in = {};
	DECLARE_SCMI_HAILO_OUT(scmi_hailo_get_sku_id_p2a, out);
	struct scmi_msg msg = SCMI_MSG_IN(SCMI_PROTOCOL_ID_HAILO,
					  SCMI_HAILO_GET_SKU_ID, in, out);

	ret = devm_scmi_process_msg(dev, &msg);
	if (ret)
		return ret;

	if (out.status)
		return scmi_to_linux_errno(out.status);

	sku_id->product = out.response.product;
	sku_id->board = out.response.board;

	return 0;
}