# SmartLib
A Smart Library System is based on the Qt platform(Version 5.0) ,using Mysql Database(Version 5.4)

A Smart Library System application including four catalogues as list:

1.WebQRC:A web application for generating QR Code.

2.WebLogin: A web application for automatic logon through a website

3.Server:A server qt application,it can accecp the connection from a client ,it's just for admin to login,but admin has the highest power.

4.Client:A client qt aplication,admin and normal users can login both,it can send a connecting request to the server application and 
all operations already be done here must be confirmed by the running server application. 

You may open the Server and Client application with Qt5.0 or a higher version,and the WebLogin and WebQRC with vs2013 or a higher version.
If you want to show some datas and check out whether the application can work out,you may install a local database to support it,mysql5.4 is g good choice.

It based on the Qt platform(Version 5.0) ,using Mysql Database(Version 5.4), has many functions such as the usual add,delete,update,search of readers and books and so on.
You can manage plenty of informations of readers and books through this system in a very convenient way,
and work efficient.What's improtant,The system can be easily upgraded,if you have you own server,
you just can arrange the WebQRC and WebLogin application on your server.After that,users can generate QR Code for themselves or books(WebQRC's function),
and then automatically logon the system by scanning the QR Code for readers through their own mobile phone(WebLogin's function).
By scanning the QR Code for books,the details of books show up right now.

The system still have some demerits and need to modify.

Thanks for your support.
