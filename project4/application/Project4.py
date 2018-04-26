import wx
import wx.xrc
import wx.adv
import os

class Project4 ( wx.Frame ):
    def __init__( self ):
        wx.Frame.__init__ ( self, None, id = wx.ID_ANY, title = "Project 4 Logging Applicaiton", pos = wx.DefaultPosition, size = wx.Size( 640,360 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )

        main_sizer = wx.BoxSizer( wx.VERTICAL )

        self.recieve_sizer = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY )
        main_sizer.Add( self.recieve_sizer, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP, 5 )

        send_sizer = wx.BoxSizer( wx.HORIZONTAL )

        self.send_txt = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_txt, 1, wx.ALL|wx.EXPAND, 5 )

        self.send_btn = wx.Button( self, wx.ID_ANY, u"Send", wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_btn, 0, wx.ALIGN_CENTER_VERTICAL|wx.BOTTOM|wx.EXPAND|wx.RIGHT|wx.TOP, 5 )


        main_sizer.Add( send_sizer, 0, wx.EXPAND, 5 )


        self.SetSizer( main_sizer )
        self.Layout()
        self.statusbar = self.CreateStatusBar( 1, wx.CAPTION, wx.ID_ANY )
        self.menubar = wx.MenuBar( 0 )
        self.logging_menu = wx.Menu()
        self.menubar.Append( self.logging_menu, u"Logging" )

        self.com_menu = wx.Menu()
        self.menubar.Append( self.com_menu, u"Communication" )

        self.SetMenuBar( self.menubar )

        if os.path.exists(os.path.join(os.getcwd(), 'mainicon.ico')):
            self.favicon = wx.Icon(os.path.join(os.getcwd(), 'mainicon.ico'), wx.BITMAP_TYPE_ICO)
        else:
            self.favicon = wx.Icon(os.path.join(getattr(sys, '_MEIPASS', os.getcwd()), 'mainicon.ico'), wx.BITMAP_TYPE_ICO)
        self.SetIcon(self.favicon)
        self.tb_icon = wx.adv.TaskBarIcon()
        self.tb_icon.SetIcon(self.favicon, "Project 4 Logging Appliction")


        self.Centre( wx.BOTH )

    def __del__( self ):
        pass

"""
Jump to the main application
"""
if __name__ == "__main__":
    app = wx.App(None)
    boardui = Project4().Show()
    app.MainLoop()
