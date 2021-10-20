## TODO:
1.~~ Prevent duplicate selected ranges.~~
2.~~ Prevent selected ranges outside the range.~~
3.~~ Remove any timestamp hardcoding.~~
4.~~ Add color Preferences.~~
5. Add file browser to all the user to read csv files.
6. Add help/How to use window
7.~~ Allow the user to select scatter or line~~
8. Convert unix time to normal date and display it in the selected ranges table
9. Use imgui window auto resizing instead of resizing the window manually
10. Add popup window when the user try to select wrong ranges
11. Functionality to add label to the selected ranges either normal, upnormal, turned off, etc...
12. divide the UI into components

## Bugs
1. When selecting small ranges and select a large range contains all the small ranges the program doesn't work as expected.
2. When selecting a large range that start at the first date and end at the last date the program crashes. The reason is the 
floorUnixTime and ceilUnixTime generate indices outside the normal data indices.

### Info:
* RapidMiner date format MM/DD/YY HH:MM AM/PM
* RapidMiner give the value precision to the 9th decimal