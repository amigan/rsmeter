#!/usr/local/bin/wish8.4
# Change the open line below to reference your serial device.
proc every {ms body}  { eval $body; after $ms [ info level 0] }
proc dowhatnot fhh {
puts -nonewline $fhh "D"
flush $fhh
}
set fhn [ open "/dev/cuaa0" "r+" ]
fconfigure $fhn -mode 1200,n,7,2 -handshake none -ttycontrol {RTS 0} 
frame .rc -borderwidth 2 -width 80c -height 24c
pack .rc
wm title . "RSMeter"
frame .mnubar -relief raised -bd 2
pack .mnubar -in .rc -fill x -expand yes
menubutton .mnubar.file -underline 0 -text "File" -menu .mnubar.file.menu
pack .mnubar.file -side left
menubutton .mnubar.help -underline 0 -text "Help" -menu .mnubar.help.menu
pack .mnubar.help -side right
label .rc.slb -width 24 -height 2
pack .rc.slb
menu .mnubar.file.menu
.mnubar.file.menu add command -label "Exit" -command {close $fhn; exit }
menu .mnubar.help.menu
.mnubar.help.menu add command -label "About" -command {message .msg -text "RSMeter.\n(C)2004, Dan Ponte" }
fileevent $fhn readable {.rc.slb configure -text [gets $fhn] }
every 1000 [list dowhatnot $fhn]
