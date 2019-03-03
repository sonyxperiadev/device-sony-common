<compatibility-matrix version="1.0" type="framework">
    <hal format="hidl" optional="true">
        <name>com.qualcomm.qti.uceservice</name>
        <version>2.0</version>
        <interface>
            <name>IUceService</name>
            <instance>com.qualcomm.qti.uceservice</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>com.quicinc.cne.api</name>
        <version>1.1</version>
        <interface>
            <name>IApiService</name>
            <instance>cnd</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>com.quicinc.cne.server</name>
        <version>2.1</version>
        <interface>
            <name>IServer</name>
            <instance>cnd</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>hardware.google.media.c2</name>
        <version>1.0</version>
        <interface>
            <name>IComponentStore</name>
            <instance>default</instance>
            <instance>software</instance>
        </interface>
        <interface>
            <name>IConfigurable</name>
            <instance>default</instance>
            <instance>software</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.display.config</name>
        <version>1.0</version>
        <interface>
            <name>IDisplayConfig</name>
            <instance>default</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.nxp.nxpese</name>
        <version>1.0</version>
        <interface>
            <name>INxpEse</name>
            <instance>default</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.nxp.nxpnfc</name>
        <version>1.0</version>
        <interface>
            <name>INxpNfc</name>
            <instance>default</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.am</name>
        <version>1.0</version>
        <interface>
            <name>IQcRilAudio</name>
            <instance>slot1</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.atcmdfwd</name>
        <version>1.0</version>
        <interface>
            <name>IAtCmdFwd</name>
            <instance>AtCmdFwdService</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.ims</name>
        <version>1.2</version>
        <interface>
            <name>IImsRadio</name>
            <instance>imsradio0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.lpa</name>
        <version>1.0</version>
        <interface>
            <name>IUimLpa</name>
            <instance>UimLpa0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.qcrilhook</name>
        <version>1.0</version>
        <interface>
            <name>IQtiOemHook</name>
            <instance>oemhook0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.uim</name>
        <version>1.0</version>
        <interface>
            <name>IUim</name>
            <instance>Uim0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.uim_remote_client</name>
        <version>1.0</version>
        <interface>
            <name>IUimRemoteServiceClient</name>
            <instance>uimRemoteClient0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.radio.uim_remote_server</name>
        <version>1.0</version>
        <interface>
            <name>IUimRemoteServiceServer</name>
            <instance>uimRemoteServer0</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.hardware.tui_comm</name>
        <version>1.0</version>
        <interface>
            <name>ITuiComm</name>
            <instance>default</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.imsrtpservice</name>
        <version>1.0</version>
        <interface>
            <name>IRTPService</name>
            <instance>imsrtpservice</instance>
        </interface>
    </hal>
    <hal format="hidl" optional="true">
        <name>vendor.qti.ims.callinfo</name>
        <transport>hwbinder</transport>
        <version>1.0</version>
        <interface>
            <name>IService</name>
            <instance>default</instance>
        </interface>
    </hal>
</compatibility-matrix>
