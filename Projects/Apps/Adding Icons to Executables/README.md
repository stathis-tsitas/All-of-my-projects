# This is an test app I made to see how we can change icons in executable programs

## If you want to add a custom icon:

**Add your .ico file in the folder**
**Rename the name in the " " in the assets.rc file to your icon name, ex: MyIcon.ico**


## To compile and run: 

If you have changed the icon first do:
 **windres assets.rc -o assets.o**

*Otherwise do: 
**gcc main.c assets.o -o TestApp.exe**
**Do .\TestApp.exe to run the actual program or double-click the executable**

*Stathis Tsitas*