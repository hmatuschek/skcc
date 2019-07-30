set(skcc_SOURCES main.cc locator.cc bands.cc
    cccluster.cc spottable.cc rbn.cc skcc.cc popup.cc mainwindow.cc settings.cc settingsdialog.cc
	application.cc webfile.cc dxcc.cc menu.cc trayicon.cc colorbutton.cc adi.cc map.cc
	selfspotmap.cc)
set(skcc_MOC_HEADERS
    cccluster.hh spottable.hh rbn.hh skcc.hh popup.hh mainwindow.hh settings.hh settingsdialog.hh
	application.hh webfile.hh dxcc.hh menu.hh trayicon.hh colorbutton.hh adi.hh map.hh
	selfspotmap.hh)
set(skcc_HEADERS locator.hh bands.hh ${skcc_MOC_HEADERS})

qt5_wrap_cpp(skcc_MOC_SOURCES ${skcc_MOC_HEADERS})
qt5_add_resources(skcc_RCC_SOURCES ../resources.qrc)

add_executable(skcc WIN32 ${skcc_SOURCES} ${skcc_MOC_SOURCES} ${skcc_RCC_SOURCES})
target_link_libraries(skcc ${LIBS})

qt5_wrap_cpp(map_MOC_SOURCES map.hh)
qt5_add_resources(map_RCC_SOURCES ../resources.qrc)

add_executable(map WIN32 maptest.cc map.cc locator.cc ${map_MOC_SOURCES} ${map_RCC_SOURCES})
target_link_libraries(map ${LIBS})


install(TARGETS skcc DESTINATION bin)

# Create "Application Bundle" under MacOS X
if(UNIX AND APPLE)
 install(TARGETS skcc DESTINATION /Applications/SKCC.app/Contents/MacOS)
 install(FILES ../dist/macosx/skcc.icns DESTINATION /Applications/SKCC.app/Contents)
 install(FILES ../dist/macosx/Info.plist DESTINATION /Applications/SKCC.app/Contents)
 install(FILES ../shared/world_01_5k.png ../shared/world_02_5k.png ../shared/world_03_5k.png
	           ../shared/world_04_5k.png ../shared/world_05_5k.png ../shared/world_06_5k.png
			   ../shared/world_07_5k.png ../shared/world_08_5k.png ../shared/world_09_5k.png
			   ../shared/world_10_5k.png ../shared/world_11_5k.png ../shared/world_12_5k.png
	     DESTINATION /Applications/SKCC.app/Contents/Resources)
endif(UNIX AND APPLE)
