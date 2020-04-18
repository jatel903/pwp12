# Automatické ovládanie čerpadla dezinfekčnej kabíny
### PWP12
###### v1.0
- Pre dosku PRESEN-FSR v1.0, ktorá je kompatibilná s Arduino NANO  
- Použitý snímač: PIR senzor prispôsobený na tento účel. Môže sa použiť aj iný senzor, ako napr. infračervená závora.

### Funkčnosť programu:
###### Program má dva režimy:
- prevádzkový režim: DIP1-OFF
- testovací režim: DIP1-ON  

###### Prevádzkový režim:
Program reaguje na nábežnú hranu snímača PIR na vstupe 1 (A1). Po tejto hrane sa zopne relé určené na spínanie čerpadla trysiek dezinfekčnej kabíny. Zároveň sa naštartuje časovač tmrRUN ktorý je aktuálne nastavený na 5s. Po pretečení časovača sa vypne relé a spustí sa časovač tmrSTOP. Ten je aktuálne nastavený na 5s. Počas tohto intervalu program ignoruje vstupné pulzy. Je to doba na opustenie kabíny po dezinfekcii.  
Program posiela na sériový port údaje ako názov, verzia, nastavené intervaly a počet uskutočnených dezinfekčných cýklov. Pre pripojenie k PC je potrebný prevodník USB -> RS TTL. Komunikačné parametre sú: **9600 1 N**  
###### Testovací režim:
V tomto režime relé kopíruje stav senzoru. Takto je možné zistiť reálne správanie senzora. 
