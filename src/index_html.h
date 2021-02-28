// Page html
const char index_html[] PROGMEM = R"=====(
<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Commande LED</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0, minimal-ui">
        <link rel="stylesheet" href="/milligram.min.css">
    </head>
    <body>
        <h1>Etat de la LED</h1>
        <h2 id="etatLED">LED %LED%</h2>
        <h1>Commande de la LED</h1>
        <div class="container">
            <div class="row">
                <div class="column column-50">
                    <button class="button" onclick="appelServeur('/switchLedOn', traiteReponse)">Allumer la LED</button>
                </div>
                <div class="column column-50">
                    <button class="button button-outline" onclick="appelServeur('/switchLedOff', traiteReponse)">Eteindre la LED</button>
                </div>
            </div>        
        </div>

        <script>
            function appelServeur(url, cFonction) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        cFonction(this);
                    }
                };
                xhttp.open("GET", url, true);
                xhttp.send();
            }
            function traiteReponse(xhttp) {
                document.getElementById("etatLED").innerHTML = "LED " + (xhttp.responseText == "1" ? "allumée":"éteinte");
            }
        </script>
    </body>
</html>
)=====";