To run AudioServer.pd, you need Pd-extended.

Get it from here: http://puredata.info/downloads

It sends by default to the localhost on port 12345.

To change the network settings you need to:
1. open the patch by double clicking it in the finder, 
2. click on the box that says [pd osc] and a window will pop up. 
3. Put the window in edit mode by pressing cmd-E, 
4. then click on the box that says [connect localhost 12345] and change the details. 
5. Save everything close both the osc window and AudioServer.pd window and reopen.
 
OSCLog12345 is a simple processing sketch for looking at OSC, good for debugging the Pd AudioServer patch.