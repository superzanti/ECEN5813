# -*- mode: python -*-

block_cipher = None


a = Analysis(['Project4.py'],
             pathex=['.'],
             binaries=[],
             datas=[ ('./mainicon.ico', '.') ],
             hiddenimports=['wx.adv', 'wx.xml'],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name='Project4',
          debug=False,
          strip=False,
          upx=True,
          console=False, icon='mainicon.ico')
