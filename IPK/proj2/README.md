# IPK - projekt 2 - ZETA

 - Projekt slouží k zachycení packetů, který byl navržen hlavně pro rozhraní ethernet. Program dokáže správně zachytit packety protokolu TCP/UDP, ostatní nebyly testovány. Testování proběhlo pouze na IPv4, ale projekt by měl podporovat i IPv6, který jsem se marně snažil otestovat. Celý projekt byl inspirován wiresharkem.
 - Omezení projektu se vztahují hlavně na IPv6, kde bylo zkoušeno pouze rozhraní lo a to narozdíl od IPv4 neprošlo kvůli jiné velikosti hlavičky packetu. Omezení na délku packetu by neměla existovat, jelikož je nastavena na největší možnou velikost a to 65535 bajtů. Dále jsem narazil na problém při encapsulaci paketu na linux specific rozhraní any, kde data byla posunutá o 2 bajty a prvních 8 bajtů bylo změněno asi nějakou maskou.
