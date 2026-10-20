#pragma once
#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Panel Praktikum IoT</title>
<style>
:root{
  --paper:#E8EDF1;
  --panel:#F6F8FA;
  --ink:#16222E;
  --muted:#58687A;
  --glass:#D3DCE4;
  --cool:#1F7A8C;
  --hot:#D2402D;
}
*{box-sizing:border-box}
[hidden]{display:none!important}
html{background:var(--paper)}
body{
  margin:0;color:var(--ink);
  font:16px/1.5 "Segoe UI",system-ui,-apple-system,Roboto,"Helvetica Neue",sans-serif;
}
.wrap{max-width:900px;margin:0 auto;padding:28px 20px 56px}

header{
  display:flex;justify-content:space-between;align-items:center;
  gap:12px;flex-wrap:wrap;margin-bottom:32px;
}
h1{font-size:1.35rem;margin:0;font-weight:700;letter-spacing:-.01em}
h2{font-size:1.05rem;margin:0 0 8px;font-weight:700}
p{margin:0}

.link{display:inline-flex;align-items:center;gap:8px;font-size:.9rem;color:var(--muted)}
.link i{width:10px;height:10px;border-radius:50%;background:#9AA7B3}
.link[data-s="ok"] i{background:var(--cool)}
.link[data-s="warn"] i{background:#C98A00}
.link[data-s="off"] i{background:var(--hot)}

.layout{display:grid;grid-template-columns:340px 1fr;gap:48px;align-items:start}

/* Panel termometer */
.instrument{
  --merc:var(--cool);
  background:var(--panel);border-radius:14px;padding:26px 28px 22px;
}
.instrument[data-hot="true"]{--merc:var(--hot)}
.reading{display:flex;align-items:baseline;gap:6px}
.num{
  font-family:"Bahnschrift","DIN Alternate","Roboto Condensed","Arial Narrow",sans-serif;
  font-weight:600;font-size:4.6rem;line-height:1;letter-spacing:-.02em;
  font-variant-numeric:tabular-nums;
}
.unit{font-size:1.4rem;color:var(--muted)}
.humid{color:var(--muted);margin-top:6px}
.humid b{color:var(--ink);font-weight:600}

.stage{position:relative;width:190px;height:240px;margin:30px 0 64px 12px}
.tube{
  position:absolute;left:0;top:0;bottom:0;width:30px;
  background:var(--glass);border-radius:15px 15px 0 0;
}
.mercury{
  position:absolute;left:10px;right:10px;bottom:0;height:0;
  background:var(--merc);transition:height .6s ease,background .3s;
}
.bulb{
  position:absolute;left:-11px;bottom:-40px;width:52px;height:52px;
  border-radius:50%;background:var(--merc);transition:background .3s;
}
.tick{
  position:absolute;left:46px;transform:translateY(50%);
  font-size:.78rem;line-height:1;color:var(--muted);
}
.tick::before{
  content:"";position:absolute;left:-13px;top:50%;width:8px;
  border-top:1.5px solid var(--muted);
}
.mark{
  position:absolute;left:-8px;width:180px;height:0;
  border-top:2px solid var(--ink);transition:bottom .4s ease;
}
.mark span{
  position:absolute;right:0;bottom:4px;padding:0 4px;
  font-size:.78rem;font-weight:600;background:var(--panel);
}

/* Kontrol */
.controls section{margin-bottom:36px}
.desc{color:var(--muted);margin-top:10px;max-width:46ch}

.notice{
  background:#FBE9E5;color:#7A2318;border-radius:8px;
  padding:10px 14px;margin-bottom:28px;max-width:52ch;
}
.notice:empty{display:none}

.seg{display:inline-flex;border:1.5px solid var(--ink);border-radius:8px;overflow:hidden}
.seg button{
  border:0;background:transparent;color:var(--ink);font:inherit;font-weight:600;
  padding:10px 24px;cursor:pointer;
}
.seg button+button{border-left:1.5px solid var(--ink)}
.seg button[aria-pressed="true"]{background:var(--ink);color:var(--paper)}

.ledrow{display:flex;align-items:center;gap:16px;margin-bottom:16px}
.lamp{
  width:52px;height:52px;border-radius:50%;flex:none;
  background:var(--glass);border:2px solid #AEBAC5;
  transition:background .2s,box-shadow .2s,border-color .2s;
}
.lamp[data-on="true"]{
  background:var(--hot);border-color:#A52F20;
  box-shadow:0 0 0 6px rgba(210,64,45,.18),0 0 26px rgba(210,64,45,.55);
}
.ledrow strong{display:block;font-size:1.1rem}
.ledrow span{color:var(--muted);font-size:.92rem}

.btn{
  border:1.5px solid var(--ink);background:transparent;color:var(--ink);
  font:inherit;font-weight:600;padding:10px 28px;border-radius:8px;cursor:pointer;
}
.btn[aria-pressed="true"]{background:var(--ink);color:var(--paper)}
.btn:disabled{opacity:.4;cursor:not-allowed}
.btnrow{display:flex;gap:10px}
.lock{color:var(--muted);font-size:.92rem;margin-top:10px}

.throw{display:flex;gap:10px;align-items:flex-end;flex-wrap:wrap}
label{display:block;font-size:.92rem;color:var(--muted);margin-bottom:4px}
input[type=number]{
  width:110px;font:inherit;padding:9px 10px;
  border:1.5px solid var(--ink);border-radius:8px;background:#fff;color:var(--ink);
}
.save{
  font:inherit;font-weight:600;padding:10px 20px;border-radius:8px;cursor:pointer;
  border:1.5px solid var(--cool);background:var(--cool);color:#fff;
}

:focus-visible{outline:3px solid var(--cool);outline-offset:2px}
@media (max-width:760px){
  .layout{grid-template-columns:1fr;gap:32px}
}
@media (prefers-reduced-motion:reduce){
  *{transition:none!important}
}
</style>
</head>
<body>
<div class="wrap">

  <header>
    <h1>Panel praktikum IoT</h1>
    <span class="link" id="link" data-s=""><i></i><span id="link-text">Menghubungkan...</span></span>
  </header>

  <div class="layout">

    <section class="instrument" id="gauge" data-hot="false" aria-label="Pembacaan sensor DHT11">
      <div class="reading"><span class="num" id="temp">--</span><span class="unit">°C</span></div>
      <p class="humid">Kelembapan udara <b id="hum">--</b>%</p>

      <div class="stage" aria-hidden="true">
        <div class="tube"><div class="mercury" id="mercury"></div></div>
        <div class="bulb"></div>
        <span class="tick" style="bottom:0%">0</span>
        <span class="tick" style="bottom:20%">10</span>
        <span class="tick" style="bottom:40%">20</span>
        <span class="tick" style="bottom:60%">30</span>
        <span class="tick" style="bottom:80%">40</span>
        <span class="tick" style="bottom:100%">50</span>
        <div class="mark" id="mark" style="bottom:60%"><span id="mark-text">Ambang 30°C</span></div>
      </div>
    </section>

    <div class="controls">
      <p class="notice" id="notice" role="status"></p>

      <section>
        <h2>Mode</h2>
        <div class="seg" role="group" aria-label="Pilih mode">
          <button type="button" id="mode-manual" aria-pressed="true">Manual</button>
          <button type="button" id="mode-auto" aria-pressed="false">Otomatis</button>
        </div>
        <p class="desc" id="mode-desc"></p>
      </section>

      <section>
        <h2>LED</h2>
        <div class="ledrow">
          <div class="lamp" id="lamp" data-on="false"></div>
          <div>
            <strong id="led-text">LED mati</strong>
            <span id="led-src"></span>
          </div>
        </div>
        <div class="btnrow">
          <button type="button" class="btn" id="led-on" aria-pressed="false">ON</button>
          <button type="button" class="btn" id="led-off" aria-pressed="true">OFF</button>
        </div>
        <p class="lock" id="lock-note" hidden>Tombol terkunci karena mode otomatis aktif. Pindah ke mode Manual untuk mengendalikan LED sendiri.</p>
      </section>

      <section>
        <h2>Ambang suhu</h2>
        <form class="throw" id="th-form">
          <div>
            <label for="th">Dipakai pada mode otomatis (0 sampai 50 °C)</label>
            <input type="number" id="th" min="0" max="50" step="0.5" value="30" required>
          </div>
          <button type="submit" class="save">Simpan ambang</button>
        </form>
      </section>
    </div>

  </div>
</div>

<script>
(function () {
  var $ = function (id) { return document.getElementById(id); };
  var POLL_MS = 1500;

  function clamp(v, a, b) { return Math.min(b, Math.max(a, v)); }
  function fmt(n) { return String(+Number(n).toFixed(1)); }

  function setLink(state, text) {
    $('link').setAttribute('data-s', state);
    $('link-text').textContent = text;
  }

  function render(d) {
    var auto = d.mode === 'AUTO';
    var online = d.uno;
    var valid = online && d.ok;

    // Status koneksi dan pesan
    if (!online) {
      setLink('warn', 'Uno tidak mengirim data');
      $('notice').textContent = 'ESP32 menyala, tetapi Arduino Uno belum mengirim data. Periksa kabel pin 10 dan 11, GND bersama, lalu pastikan sketch Uno sudah diunggah.';
    } else if (!d.ok) {
      setLink('ok', 'Uno terhubung');
      $('notice').textContent = 'Sensor DHT11 gagal dibaca. Periksa kabel data di pin 2 Uno.';
    } else {
      setLink('ok', 'Uno terhubung');
      $('notice').textContent = '';
    }

    // Termometer
    $('temp').textContent = valid ? d.temp.toFixed(1) : '--';
    $('hum').textContent = valid ? d.hum.toFixed(0) : '--';
    $('mercury').style.height = (valid ? clamp(d.temp / 50 * 100, 0, 100) : 0) + '%';
    $('mark').style.bottom = clamp(d.th / 50 * 100, 0, 100) + '%';
    $('mark-text').textContent = 'Ambang ' + fmt(d.th) + '°C';
    $('gauge').setAttribute('data-hot', String(valid && d.temp >= d.th));

    // Mode
    $('mode-manual').setAttribute('aria-pressed', String(!auto));
    $('mode-auto').setAttribute('aria-pressed', String(auto));
    $('mode-desc').textContent = auto
      ? 'LED dikendalikan suhu. LED menyala saat suhu mencapai ' + fmt(d.th) + '°C atau lebih, dan mati jika di bawahnya.'
      : 'LED mengikuti tombol ON dan OFF. Suhu hanya ditampilkan dan tidak memengaruhi LED.';

    // LED
    var on = d.led === 1;
    $('lamp').setAttribute('data-on', String(on));
    $('led-text').textContent = on ? 'LED menyala' : 'LED mati';
    $('led-src').textContent = auto ? 'Diatur oleh suhu' : 'Diatur dari tombol';
    $('led-on').setAttribute('aria-pressed', String(on));
    $('led-off').setAttribute('aria-pressed', String(!on));
    $('led-on').disabled = $('led-off').disabled = auto || !online;
    $('lock-note').hidden = !auto;

    if (document.activeElement !== $('th')) { $('th').value = fmt(d.th); }
  }

  function refresh() {
    var ctl = new AbortController();
    var timer = setTimeout(function () { ctl.abort(); }, 3000);
    fetch('/status', { cache: 'no-store', signal: ctl.signal })
      .then(function (r) { if (!r.ok) throw new Error(r.status); return r.json(); })
      .then(render)
      .catch(function () {
        setLink('off', 'ESP32 tidak terjangkau');
        $('notice').textContent = 'Browser tidak bisa menghubungi ESP32. Pastikan perangkat ini terhubung ke WiFi yang sama dengan ESP32.';
      })
      .then(function () { clearTimeout(timer); });
  }

  function send(query) {
    fetch('/set?' + query, { cache: 'no-store' })
      .then(function (r) { if (!r.ok) throw new Error(r.status); setTimeout(refresh, 350); })
      .catch(function () { $('notice').textContent = 'Perintah gagal dikirim ke ESP32.'; });
  }

  $('mode-manual').addEventListener('click', function () { send('mode=manual'); });
  $('mode-auto').addEventListener('click', function () { send('mode=auto'); });
  $('led-on').addEventListener('click', function () { send('led=on'); });
  $('led-off').addEventListener('click', function () { send('led=off'); });
  $('th-form').addEventListener('submit', function (e) {
    e.preventDefault();
    send('th=' + parseFloat($('th').value));
    $('th').blur();
  });

  refresh();
  setInterval(refresh, POLL_MS);
})();
</script>
</body>
</html>
)rawliteral";
