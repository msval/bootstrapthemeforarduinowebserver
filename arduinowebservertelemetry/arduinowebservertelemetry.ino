#include <EtherCard.h>

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,1,203 };

byte Ethernet::buffer[1000];
BufferFiller bfill;

void setup () {
  ether.begin(sizeof Ethernet::buffer, mymac);
  ether.staticSetup(myip);
}

static word homePage() {
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<title>Arduino home web server</title>"
    "<link rel=\"stylesheet\" href=\"http://netdna.bootstrapcdn.com/bootstrap/3.1.1/css/bootstrap.min.css\">"
    "<style>"
    "body{padding-top:20px;padding-bottom:20px}.header{border-bottom:1px solid #e5e5e5;margin-bottom:0}"
    ".jumbotron{text-align:center}.marketing{margin:40px 0}"
    ".arduino h4{color:#2ecc71;margin-top:28px;padding-right:0;padding-left:0}"
    "</style>"
    "</head>"
    "<body>"
    "<div class=\"container\">"
    "<div class=\"header\">"
    "<h3 class=\"text-muted\">Arduino home server</h3>"
    "</div>"
    "<div class=\"row arduino\">"
    "<div class=\"col-lg-6\">"
    "<h4>Temperature</h4>"
    "<p>$D °C</p>"
    "<h4>Humidity</h4>"
    "<p>$D%</p>"
    "</div>"
    "</div>"
    "</div>"
    "</body>"
    "</html>"), 20, 30);
  return bfill.position();
}

void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)
    ether.httpServerReply(homePage());
}
