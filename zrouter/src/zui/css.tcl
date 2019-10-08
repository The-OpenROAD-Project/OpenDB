package provide ade::ui 2.0

#      Author: Manolis M. Tsangaris


proc css_setup {} {
    set f1 [font create -family helvetica -size 10 -weight bold -slant italic]
    set f2 [font create -family courier -size 10]
    set f3 [font create -family helvetica -size 8]
    set f4 [font create -family helvetica -size 8 -weight bold]

    option add *Menu.font            $f1
    option add *Menubutton.font      $f1
    option add *Button.font          $f1
    option add *LPopupText*Text*font $f2
    option add *LMenu.font           $f1
    option add *LLinearToolBar.font  $f1

    option add *CPanel.status.font  $f3

    option add *Filter*Label.background gray80
    option add *Filter*Frame.background gray75
    option add *filter*font    $f3

    option add *ZUI.LConsole*font  "courier 8"
    option add *ZConsole*font  $f3

    option add *MSHViewer*font $f3

    option add *CPanel*font $f4
    option add *LSTree*font $f3

}
