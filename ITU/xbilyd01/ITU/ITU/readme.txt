Pro funkci programu není potřeba připojovat žádnou externí knihovnu
(pouze se některé vyžadují z kódu, např jQuery - ovšem pro jejich přidání stačí mít připojení k internetu).

Program je uložen na serveru FIT Eva k okamžitému spuštění. Je přístupný na adrese:
http://www.stud.fit.vutbr.cz/~xbilyd01/ITU


V případě nového uživatele je potřeba nejprve vytvořit nový účet(pozn. ve finální verzi je možné vytvořit účet pouze na verzi http://www.stud.fit.vutbr.cz/~xmarek69/ITU)

Jinak je možné se přímo přihlásit a aplikaci využívat. Pro testování je předem připraven účet s přihlašovacími údaji:

Přihlašovací jméno: a
Heslo: a

V případě že by bylo potřeba nainstalovat aplikaci na jiný web, bylo by potřeba nejprve vytvořit tabulky v databázi pomocí skriptu s názvem databaze.sql (v kořenovém adresáři odevzdaného archivu). Rovněž by bylo potřeba změnit řádek kódu (doplnit správné údaje k připojení k databázi), kde se v souboru index.php volá funkce pripoj (nad třídou Db).
