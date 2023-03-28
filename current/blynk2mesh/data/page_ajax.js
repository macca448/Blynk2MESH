  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("mytime").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/mytime", true);
    xhttp.send();
  }, 1000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("sysalarm").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/sysalarms", true);
    xhttp.send();
  }, 5000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temps").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/systemps", true);
    xhttp.send();
  }, 5000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("hums").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/syshums", true);
    xhttp.send();
  }, 5000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("press").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/syspres", true);
    xhttp.send();
  }, 5000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("errors").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/errors", true);
    xhttp.send();
  }, 5000 ) ;  

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("light").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/light", true);
    xhttp.send();
  }, 5000 ) ; 

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("doors").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/doors", true);
    xhttp.send();
  }, 5000 ) ;  

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("mouse").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/mouse", true);
    xhttp.send();
  }, 5000 ) ;  

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("da").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/galarm", true);
    xhttp.send();
  }, 5000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("sa").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/salarms", true);
    xhttp.send();
  }, 5000 ) ;  

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("valve").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/vpos", true);
    xhttp.send();
  }, 5000 ) ; 
  