#include <Arduino.h>
const char INDEX_HTML[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Telescope Control</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
            body{font-family:Arial,sans-serif;margin:20px;background:#f0f0f0}
            .card{background:white;padding:20px;border-radius:8px;margin-bottom:20px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}
            .btn{background:#007bff;color:white;border:none;padding:10px 20px;border-radius:4px;cursor:pointer}
            .btn:hover{background:#0056b3}
            input{padding:8px;margin:5px 0;border:1px solid #ddd;border-radius:4px;width:200px}
        </style>
    </head>
    <body>
        <div class="card">
            <h2>Telescope Status</h2>
            <p>RA: <span id="ra">--</span></p>
            <p>DEC: <span id="dec">--</span></p>
            <p>Latitude: <span id="lat">--</span></p>
            <p>Longitude: <span id="lon">--</span></p>
            <p>Altitude: <span id="alt">--</span></p>
        </div>
    
        <div class="card">
            <h2>Goto Command</h2>
            <input type="number" id="gotoRA" step="0.01" placeholder="RA">
            <input type="number" id="gotoDEC" step="0.01" placeholder="DEC">
            <button class="btn" onclick="sendGoto()">GOTO</button>
        </div>
    
        <div class="card">
            <h2>WiFi Settings</h2>
            <input type="text" id="ssid" placeholder="SSID">
            <input type="password" id="password" placeholder="Password">
            <button class="btn" onclick="saveWifi()">Save</button>
        </div>
    
        <div class="card">
            <h2>System</h2>
            <button class="btn" onclick="reboot()">Reboot</button>
        </div>
    
        <script>
            function updateStatus(){fetch('/api/get?d=telescope').then(r=>r.json()).then(d=>{
                document.getElementById('ra').textContent=d.d.TEL.ra.toFixed(2);
                document.getElementById('dec').textContent=d.d.TEL.dec.toFixed(2);
                document.getElementById('lat').textContent=d.d.LATITUDE.toFixed(2);
                document.getElementById('lon').textContent=d.d.LONGITUD.toFixed(2);
                document.getElementById('alt').textContent=d.d.ALTITUDE.toFixed(2);
            })}
            function sendGoto(){
                const ra=document.getElementById('gotoRA').value;
                const dec=document.getElementById('gotoDEC').value;
                fetch(`/api/cmd?d=telescope&c=move ${ra} ${dec}`).then(r=>r.json())
                .then(d=>{d.ret===0?alert('Command sent'):alert('Error')});
            }
            function saveWifi(){
                const s=document.getElementById('ssid').value;
                const p=document.getElementById('password').value;
                fetch(`/settings?ssid=${encodeURIComponent(s)}&pwd=${encodeURIComponent(p)}`)
                .then(r=>r.json()).then(d=>{alert('WiFi settings saved')});
            }
            function reboot(){
                if(confirm('Reboot?'))fetch('/reboot').then(r=>r.json()).then(d=>{alert('Rebooting...')});
            }
            setInterval(updateStatus,2000);
            updateStatus();
        </script>
    </body>
    </html>
    )rawliteral";