#pragma once
#include <pgmspace.h>

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 P3 Monitoring</title>
<style>
  :root{
    --hitam:#0e0e0e;
    --putih:#ffffff;
    --latar:#f4f3f0;
    --abu:#8b8982;
    --garis:#d9d6cf;
    --aksen:#e03a2b;
  }
  *{box-sizing:border-box;margin:0;padding:0}
  body{
    background:var(--latar);
    color:var(--hitam);
    font-family:"Helvetica Neue",Helvetica,Arial,sans-serif;
    -webkit-font-smoothing:antialiased;
  }
  .num{font-variant-numeric:tabular-nums;letter-spacing:-.03em}
  .kecil{
    font-size:.68rem;font-weight:600;
    text-transform:uppercase;letter-spacing:.16em;
  }

  /* ---------- bilah atas ---------- */
  .atas{
    background:var(--hitam);color:var(--putih);
    display:flex;justify-content:space-between;align-items:center;
    gap:16px;padding:16px 28px;
  }
  .atas h1{font-size:.95rem;font-weight:700;letter-spacing:.02em}
  .atas h1 span{color:var(--abu);font-weight:400;margin-left:10px}
  .kanan{display:flex;align-items:center;gap:20px}
  .kanan .kecil{color:var(--abu)}
  #koneksi{display:flex;align-items:center;gap:8px;color:var(--putih)}
  #koneksi i{width:9px;height:9px;background:var(--abu);display:block}
  #koneksi.on i{background:#4cd964}
  #koneksi.off i{background:var(--aksen)}

  main{max-width:1080px;margin:0 auto;padding:28px}

  /* ---------- pembacaan utama ---------- */
  .utama{display:grid;grid-template-columns:1fr 1fr;gap:4px}
  .panel{background:var(--hitam);color:var(--putih);padding:26px 28px 24px}
  .panel .kecil{color:var(--abu)}
  .angka{
    font-size:clamp(4rem,11vw,7.5rem);
    font-weight:700;line-height:1;margin:18px 0 6px;
  }
  .angka sup{
    font-size:.28em;font-weight:400;color:var(--abu);
    vertical-align:top;position:relative;top:.35em;margin-left:6px;letter-spacing:0;
  }
  .skala{margin-top:24px}
  .batang{position:relative;height:6px;background:#2b2b2b}
  .batang b{position:absolute;left:0;top:0;bottom:0;background:var(--putih);width:0;transition:width .6s}
  #bSuhu{background:var(--aksen)}
  .tanda{display:flex;justify-content:space-between;margin-top:8px;font-size:.7rem;color:var(--abu)}
  .minmax{
    display:flex;gap:28px;margin-top:20px;padding-top:14px;
    border-top:1px solid #2b2b2b;font-size:.8rem;color:var(--abu);
  }
  .minmax b{color:var(--putih);font-weight:600;margin-left:6px}

  /* ---------- grafik ---------- */
  .blok{background:var(--putih);border-top:4px solid var(--hitam);margin-top:28px;padding:20px 28px 18px}
  .judul{display:flex;justify-content:space-between;align-items:baseline;margin-bottom:14px}
  .judul h2{font-size:1.05rem;font-weight:700}
  .judul .kecil{color:var(--abu)}
  .grafikWrap{position:relative;padding-left:34px}
  #grafik{width:100%;height:190px;display:block}
  .sumbu{position:absolute;left:0;top:0;height:190px;display:flex;flex-direction:column;justify-content:space-between;font-size:.66rem;color:var(--abu)}
  .legenda{display:flex;gap:22px;margin-top:12px;font-size:.78rem}
  .legenda span{display:flex;align-items:center;gap:8px}
  .legenda i{width:18px;height:3px;display:block}

  /* ---------- kontrol LED ---------- */
  .led{
    margin-top:28px;display:flex;align-items:stretch;
    background:var(--putih);border-top:4px solid var(--hitam);
  }
  .ledKiri{flex:1;padding:22px 28px}
  .ledKiri .kecil{color:var(--abu)}
  #teksLed{font-size:2.2rem;font-weight:700;margin-top:6px;letter-spacing:-.02em}
  .ledKiri p{font-size:.8rem;color:var(--abu);margin-top:4px}
  .ledKanan{display:flex;align-items:center;padding:0 28px;gap:18px;border-left:1px solid var(--garis)}
  #lampu{width:20px;height:20px;border-radius:50%;border:3px solid var(--hitam);transition:.2s}
  #lampu.nyala{background:var(--aksen);border-color:var(--aksen);box-shadow:0 0 0 5px rgba(224,58,43,.18)}
  #sakelar{
    width:76px;height:38px;background:var(--hitam);border:0;
    position:relative;cursor:pointer;flex:none;
    transition:background .2s;
  }
  #sakelar::after{
    content:"";position:absolute;top:4px;left:4px;
    width:30px;height:30px;background:var(--putih);transition:transform .2s;
  }
  #sakelar.nyala{background:var(--aksen)}
  #sakelar.nyala::after{transform:translateX(38px)}
  #sakelar:focus-visible{outline:3px solid var(--aksen);outline-offset:3px}

  footer{
    margin-top:28px;padding-top:14px;border-top:1px solid var(--garis);
    display:flex;justify-content:space-between;flex-wrap:wrap;gap:8px;
    font-size:.74rem;color:var(--abu);
  }
  code{font-family:Menlo,Consolas,"Courier New",monospace;color:var(--hitam)}

  @media (max-width:720px){
    .atas{padding:14px 18px;flex-wrap:wrap}
    .atas h1 span{display:none}
    main{padding:16px}
    .utama{grid-template-columns:1fr}
    .panel{padding:20px}
    .blok{padding:18px 16px}
    .ledKiri{padding:18px 16px}
    .ledKanan{padding:0 16px}
    #teksLed{font-size:1.6rem}
  }
</style>
</head>
<body>

<div class="atas">
  <h1>P3 MONITORING &amp; CONTROLLING<span>Lab Workshop Ilmu Komputer, Universitas Pakuan</span></h1>
  <div class="kanan">
    <span class="kecil" id="alamat">ESP32</span>
    <span class="kecil" id="koneksi"><i></i><em style="font-style:normal">menghubungkan</em></span>
  </div>
</div>

<main>

  <div class="utama">
    <div class="panel">
      <div class="kecil">Suhu</div>
      <div class="angka num" id="suhu">--<sup>&deg;C</sup></div>
      <div class="skala">
        <div class="batang"><b id="bSuhu"></b></div>
        <div class="tanda num"><span>0</span><span>25</span><span>50 &deg;C</span></div>
      </div>
      <div class="minmax num">
        <span>MIN<b id="tMin">--</b></span>
        <span>MAX<b id="tMax">--</b></span>
        <span>RATA<b id="tAvg">--</b></span>
      </div>
    </div>

    <div class="panel">
      <div class="kecil">Kelembapan</div>
      <div class="angka num" id="lembap">--<sup>%</sup></div>
      <div class="skala">
        <div class="batang"><b id="bLembap"></b></div>
        <div class="tanda num"><span>0</span><span>50</span><span>100 %</span></div>
      </div>
      <div class="minmax num">
        <span>MIN<b id="hMin">--</b></span>
        <span>MAX<b id="hMax">--</b></span>
        <span>RATA<b id="hAvg">--</b></span>
      </div>
    </div>
  </div>

  <div class="blok">
    <div class="judul">
      <h2>Riwayat pembacaan</h2>
      <span class="kecil">30 data terakhir &middot; tiap 2 detik</span>
    </div>
    <div class="grafikWrap">
      <div class="sumbu num"><span>100</span><span>75</span><span>50</span><span>25</span><span>0</span></div>
      <svg id="grafik" viewBox="0 0 600 190" preserveAspectRatio="none">
        <g stroke="#d9d6cf" stroke-width="1">
          <line x1="0" y1="0.5" x2="600" y2="0.5"/>
          <line x1="0" y1="47.5" x2="600" y2="47.5"/>
          <line x1="0" y1="95" x2="600" y2="95"/>
          <line x1="0" y1="142.5" x2="600" y2="142.5"/>
          <line x1="0" y1="189.5" x2="600" y2="189.5"/>
        </g>
        <g id="garis"></g>
      </svg>
    </div>
    <div class="legenda">
      <span><i style="background:#e03a2b"></i>Suhu (skala 0&ndash;50 &deg;C)</span>
      <span><i style="background:#0e0e0e"></i>Kelembapan (skala 0&ndash;100 %)</span>
    </div>
  </div>

  <div class="led">
    <div class="ledKiri">
      <div class="kecil">Output &middot; GPIO 4</div>
      <div id="teksLed">LED MATI</div>
      <p>Geser sakelar untuk mengirim perintah ke ESP32.</p>
    </div>
    <div class="ledKanan">
      <div id="lampu"></div>
      <button id="sakelar" aria-label="Sakelar LED"></button>
    </div>
  </div>

</main>

<script>
  var $ = function(id){ return document.getElementById(id); };
  var infoKoneksi = $('koneksi');
  var ledNyala = false;
  var T = [], H = [];
  var MAKS = 30;

  $('alamat').textContent = location.host || 'ESP32';

  function tampilLed(){
    $('lampu').className = ledNyala ? 'nyala' : '';
    $('sakelar').className = ledNyala ? 'nyala' : '';
    $('teksLed').textContent = ledNyala ? 'LED MENYALA' : 'LED MATI';
  }

  function stat(arr, digit){
    var min = Math.min.apply(null, arr), max = Math.max.apply(null, arr);
    var avg = arr.reduce(function(a, b){ return a + b; }, 0) / arr.length;
    return [min.toFixed(digit), max.toFixed(digit), avg.toFixed(digit)];
  }

  function garis(data, maks, warna, tebal){
    if (data.length < 2) return '';
    var t = data.map(function(v, i){
      var x = i * (600 / (MAKS - 1));
      var y = 189 - Math.max(0, Math.min(1, v / maks)) * 188;
      return x.toFixed(1) + ',' + y.toFixed(1);
    }).join(' ');
    return '<polyline points="' + t + '" fill="none" stroke="' + warna +
           '" stroke-width="' + tebal + '" vector-effect="non-scaling-stroke" stroke-linejoin="round"/>';
  }

  function perbarui(d){
    $('suhu').innerHTML = d.temp.toFixed(1) + '<sup>&deg;C</sup>';
    $('lembap').innerHTML = d.hum.toFixed(0) + '<sup>%</sup>';
    $('bSuhu').style.width = Math.max(0, Math.min(100, d.temp * 2)) + '%';
    $('bLembap').style.width = Math.max(0, Math.min(100, d.hum)) + '%';

    T.push(d.temp); H.push(d.hum);
    if (T.length > MAKS) { T.shift(); H.shift(); }

    var s = stat(T, 1), h = stat(H, 0);
    $('tMin').textContent = s[0]; $('tMax').textContent = s[1]; $('tAvg').textContent = s[2];
    $('hMin').textContent = h[0]; $('hMax').textContent = h[1]; $('hAvg').textContent = h[2];

    $('garis').innerHTML = garis(H, 100, '#0e0e0e', 2) + garis(T, 50, '#e03a2b', 3);
  }

  function ambilData(){
    fetch('/data')
      .then(function(r){ return r.json(); })
      .then(function(d){
        if (d.error) {
          $('suhu').innerHTML = '--<sup>&deg;C</sup>';
          $('lembap').innerHTML = '--<sup>%</sup>';
        } else {
          perbarui(d);
        }
        ledNyala = d.led;
        tampilLed();
        infoKoneksi.className = 'kecil on';
        infoKoneksi.lastChild.textContent = 'terhubung';
      })
      .catch(function(){
        infoKoneksi.className = 'kecil off';
        infoKoneksi.lastChild.textContent = 'putus';
      });
  }

  $('sakelar').addEventListener('click', function(){
    var alamat = ledNyala ? '/led-off' : '/led-on';
    fetch(alamat)
      .then(function(r){ return r.json(); })
      .then(function(d){ ledNyala = d.led; tampilLed(); });
  });

  tampilLed();
  ambilData();
  setInterval(ambilData, 2000);
</script>
</body>
</html>
)rawliteral";
