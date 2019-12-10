set(rbnc_SOURCES main.cc locator.cc bands.cc icons.cc
    cccluster.cc spottable.cc rbn.cc skcc.cc popup.cc mainwindow.cc settings.cc settingsdialog.cc
	application.cc webfile.cc dxcc.cc menu.cc trayicon.cc colorbutton.cc adi.cc map.cc
  selfspotmap.cc aboutdialog.cc)
set(rbnc_MOC_HEADERS
    cccluster.hh spottable.hh rbn.hh skcc.hh popup.hh mainwindow.hh settings.hh settingsdialog.hh
	application.hh webfile.hh dxcc.hh menu.hh trayicon.hh colorbutton.hh adi.hh map.hh
  selfspotmap.hh aboutdialog.hh)
set(rbnc_HEADERS locator.hh bands.hh icons.hh ${skcc_MOC_HEADERS})

qt5_wrap_cpp(rbnc_MOC_SOURCES ${rbnc_MOC_HEADERS})
qt5_add_resources(rbnc_RCC_SOURCES ../resources.qrc)

add_executable(rbnc WIN32 ${rbnc_SOURCES} ${rbnc_MOC_SOURCES} ${rbnc_RCC_SOURCES})
target_link_libraries(rbnc ${LIBS})

# Create "Application Bundle" under MacOS X
if(UNIX AND APPLE)
 install(TARGETS rbnc DESTINATION /Applications/RBNC.app/Contents/MacOS)
 install(FILES ../dist/macosx/rbnc.icns DESTINATION /Applications/RBNC.app/Contents)
 install(FILES ../dist/macosx/Info.plist DESTINATION /Applications/RBNC.app/Contents)
 install(FILES ../shared/world_01_5k.png ../shared/world_02_5k.png ../shared/world_03_5k.png
	           ../shared/world_04_5k.png ../shared/world_05_5k.png ../shared/world_06_5k.png
			   ../shared/world_07_5k.png ../shared/world_08_5k.png ../shared/world_09_5k.png
			   ../shared/world_10_5k.png ../shared/world_11_5k.png ../shared/world_12_5k.png
       DESTINATION /Applications/RBNC.app/Contents/Resources)
elseif(UNIX AND NOT APPLE)
  install(TARGETS rbnc DESTINATION bin)
  install(FILES ../shared/world_01_5k.png ../shared/world_02_5k.png ../shared/world_03_5k.png
              ../shared/world_04_5k.png ../shared/world_05_5k.png ../shared/world_06_5k.png
          ../shared/world_07_5k.png ../shared/world_08_5k.png ../shared/world_09_5k.png
          ../shared/world_10_5k.png ../shared/world_11_5k.png ../shared/world_12_5k.png
          DESTINATION share/io.github.hmatuschek/rbnc)
  install(FILES ../dist/linux/rbnc.desktop DESTINATION share/applications)
  install(FILES ../dist/linux/rbnc.png DESTINATION share/icons)
endif(UNIX AND APPLE)
