const char HTML_MAIN[] PROGMEM = R"rawliteral(

<div class="row gx-0 mb-2">
<div class="col">
<button id="prevInv" type="button" class="btn btn-primary">&#8882;</button>
</div>
<div class="col-9">
<figure class="text-center">
    <h3 id="devicename"></h3>
</figure>
</div>
<div class="col">
<button id="nextInv" type="button" class="btn btn-primary" style="float: right;">&#8883;</button>
</div>
</div>


<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Device Time: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="devtime"></span></br></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Solar: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="solarV"></span><span id="solarA"></span><span id="solarW"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Battery: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="battV"></span><span id="battA"></span><span id="battW"></span><span
                id="battSOC"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Load: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="loadV"></span><span id="loadA"></span><span id="loadW"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Consumed Kwh: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="consD"></span><span id="consM"></span><span id="consY"></span><span
                id="consT"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">generated Kwh: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="genD"></span><span id="genM"></span><span id="genY"></span><span
                id="genT"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">CO2 Reduction: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="cored"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Input State: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="inputstate"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Charge Mode: </div>
    </div>
    <div class="col-7">
        <div class="bg-light"><span id="chrgmode"></span></div>
    </div>
</div>

<div class="row gx-0 mb-2">
    <div class="col">
        <div class="bg-light">Load State: </div>
    </div>
    <div class="col-7">
        <div class="bg-light form-check form-switch"><input class="form-check-input" type="checkbox" role="switch"
                id="loadState" /></div>
    </div>
</div>
<div class="d-grid gap-2">
    <a class="btn btn-primary btn-block" href="/settings" role="button">Settings</a>
</div>

<script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    var selInverter = 1;
    var invQuantity = 1;
    window.addEventListener('load', onLoad);
    function initWebSocket() {
        console.log('Trying to open a WebSocket connection...');
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
    }
    function onOpen(event) {
        console.log('Connection opened');
    }
    function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 2000);
    }
    function onMessage(event) {
        var data = JSON.parse(event.data);

        document.getElementById("devicename").innerHTML = data.DEVICE_NAME == null ? 'No Connection' : 'Device: ' + data.DEVICE_NAME;

        document.getElementById("devtime").innerHTML = unixTimetoDateTime(data.DEVICE_TIME);

        document.getElementById("solarV").innerHTML = data.LiveData.SOLAR_VOLTS + 'V ';
        document.getElementById("solarA").innerHTML = data.LiveData.SOLAR_AMPS + 'A  ';
        document.getElementById("solarW").innerHTML = data.LiveData.SOLAR_WATTS + 'W  ';
        document.getElementById("battSOC").innerHTML = data.LiveData.BATTERY_SOC + '%%';

        document.getElementById("battV").innerHTML = data.LiveData.BATT_VOLTS + 'V ';
        document.getElementById("battA").innerHTML = data.LiveData.BATT_AMPS + 'A  ';
        document.getElementById("battW").innerHTML = data.LiveData.BATT_WATTS + 'W  ';

        document.getElementById("loadV").innerHTML = data.LiveData.LOAD_VOLTS + 'V ';
        document.getElementById("loadA").innerHTML = data.LiveData.LOAD_AMPS + 'A  ';
        document.getElementById("loadW").innerHTML = data.LiveData.LOAD_WATTS + 'W  ';

        document.getElementById("consD").innerHTML = 'D:' + data.StatsData.CONS_ENERGY_DAY + '  ';
        document.getElementById("consM").innerHTML = 'M:' + data.StatsData.CONS_ENGERY_MON + '  ';
        document.getElementById("consY").innerHTML = 'Y:' + data.StatsData.CONS_ENGERY_YEAR + '  ';
        document.getElementById("consT").innerHTML = 'T:' + data.StatsData.CONS_ENGERY_TOT;

        document.getElementById("genD").innerHTML = 'D:' + data.StatsData.GEN_ENERGY_DAY + '  ';
        document.getElementById("genM").innerHTML = 'M:' + data.StatsData.GEN_ENERGY_MON + '  ';
        document.getElementById("genY").innerHTML = 'Y:' + data.StatsData.GEN_ENERGY_YEAR + '  ';
        document.getElementById("genT").innerHTML = 'T:' + data.StatsData.GEN_ENERGY_TOT;

        document.getElementById("cored").innerHTML = data.StatsData.CO2_REDUCTION + 't ';

        document.getElementById("inputstate").innerHTML = data.CHARGER_INPUT_STATUS;
        document.getElementById("chrgmode").innerHTML = data.CHARGER_MODE;

        document.getElementById("loadState").checked = data.LOAD_STATE;
        invQuantity = data.DEVICE_QUANTITY;
            if(invQuantity <= 1){
            document.getElementById('prevInv').style.visibility = 'hidden';
            document.getElementById('nextInv').style.visibility = 'hidden';
        }

    }

    function onLoad(event) {
        initWebSocket();
        initButton();
    }

    function initButton() {
        document.getElementById('loadState').addEventListener('click', LoadSwitch);

        document.getElementById('prevInv').addEventListener('click', SelInvp);
        document.getElementById('nextInv').addEventListener('click', SelInvn);
    }


    function LoadSwitch() {
        let loadSwitch;
        if (document.getElementById('loadState').checked) { loadSwitch = 1 }
        else { loadSwitch = 0 }

        websocket.send('loadSwitch_' + selInverter + '_' + loadSwitch);
    }

    function SelInvp() {
        if (selInverter > 1) {
            selInverter--;
            websocket.send('wsSelInv_' + selInverter);
        }
    }
    function SelInvn() {
        if (selInverter < invQuantity) {
            selInverter++;
            websocket.send('wsSelInv_' + selInverter);
        }
    }

    function unixTimetoDateTime(unixTime) {
        var deviceDate = new Date(unixTime * 1000);
        var formatedTime = (deviceDate.getFullYear()) + "-" +
            ((deviceDate.getMonth() + 1) < 10 ? '0' : '') + (deviceDate.getMonth() + 1) + "-" +
            (deviceDate.getDate() < 10 ? '0' : '') + deviceDate.getDate() + " " +
            (deviceDate.getHours() < 10 ? '0' : '') + deviceDate.getHours() + ":" +
            (deviceDate.getMinutes() < 10 ? '0' : '') + deviceDate.getMinutes() + ":" +
            (deviceDate.getSeconds() < 10 ? '0' : '') + deviceDate.getSeconds();
        return formatedTime;
    }
</script>
)rawliteral";
