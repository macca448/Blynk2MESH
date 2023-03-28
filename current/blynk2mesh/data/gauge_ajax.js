     setInterval(function() { 
     GetADC();
   }, 10500);
  
function GetADC() {    
var xhttp = new XMLHttpRequest();
          var adc=0;
     xhttp.onreadystatechange = function() {
       if (this.readyState == 4 && this.status == 200) {
          adc = Number(this.responseText);
        //adc = this.responseText ;
      document.getElementById("level").innerHTML = adc;
       gaugePS.value = adc;  //send DATA to needle
       }   
     };
     xhttp.open("GET", "/tankLevel", true);
     xhttp.send();
   }  

    var gaugePS = new RadialGauge({
    renderTo: 'gauge-ps',
        width: 250,
        height: 250,
        units: 'TANK LEVEL',
        title: false,
        value: 0,
        minValue: 0,
        maxValue: 100,
        majorTicks: [
            '0','10','20','30','40','50','60','70','80','90','100'
        ],
        minorTicks: 4,
        strokeTicks: false,
        highlights: [
            { from: 0, to: 5, color: 'rgba(255,0,0,1)' },
            { from: 5, to: 10, color: 'rgba(255,0,0,.75)' },
            { from: 10, to: 15, color: 'rgba(255,0,0,.5)' },
            { from: 15, to: 100, color: 'rgba(0,0,225,1)' }
            
        ],
        colorPlate: '#222',
        colorMajorTicks: '#f5f5f5',
        colorMinorTicks: '#ddd',
        colorTitle: '#fff',
        colorUnits: '#ccc',
        colorNumbers: '#eee',
        colorNeedle: 'rgba(240, 128, 128, 1)',
        colorNeedleEnd: 'rgba(255, 160, 122, .9)',
        valueBox: true,
	valueInt: 3,
	valueDec: 1,
        animationRule: 'bounce',
        animationDuration: 500
});
gaugePS.draw();
