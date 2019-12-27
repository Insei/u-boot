/*
 * (C) Copyright 2019
 * Insei <goodmobiledevices@gmail.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch-tegra/tegra_i2c.h>
#include "tps65913_init.h"


void tegra_i2c_ll_write_addr(uint addr, uint config)
{
	struct i2c_ctlr *reg = (struct i2c_ctlr *)TEGRA_DVC_BASE;

	writel(addr, &reg->cmd_addr0);
	writel(config, &reg->cnfg);
}

void tegra_i2c_ll_write_data(uint data, uint config)
{
	struct i2c_ctlr *reg = (struct i2c_ctlr *)TEGRA_DVC_BASE;

	writel(data, &reg->cmd_data1);
	writel(config, &reg->cnfg);
}

void pmic_enable_cpu_vdd(void)
{
	debug("%s entry\n", __func__);

	/* Don't need to set up VDD_CORE - already done - by OTP */

	debug("%s: Setting VDD_CPU to 1.0V via TPS65913\n", __func__);
	/*
	 * Bring up VDD_CPU via the TPS65913 PMIC on the PWR I2C bus.
	 * First set VDD to 1.0V, then enable the VDD regulator.
	 */
	tegra_i2c_ll_write_addr(TPS65913_I2C_ADDR, 2);
	tegra_i2c_ll_write_data(TPS65913_SMPS12_VOLTAGE_DATA, I2C_SEND_2_BYTES);
	udelay(1000);
	tegra_i2c_ll_write_data(TPS65913_SMPS12_CTRL_DATA, I2C_SEND_2_BYTES);
	udelay(10 * 1000);

	debug("%s: Setting VDD_GPU to 1.0V via TPS65913\n", __func__);
	/*
	 * Bring up VDD_GPU via the TPS65913 PMIC on the PWR I2C bus.
	 * First set VDD to 1.0V, then enable the VDD regulator.
	 */
	tegra_i2c_ll_write_addr(TPS65913_I2C_ADDR, 2);
	tegra_i2c_ll_write_data(TPS65913_SMPS45_VOLTAGE_DATA, I2C_SEND_2_BYTES);
	udelay(1000);
	tegra_i2c_ll_write_data(TPS65913_SMPS45_CTRL_DATA, I2C_SEND_2_BYTES);
	udelay(10 * 1000);
}
