PROG=proj

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/keyboard_mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video_card
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/devices/serialPort
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/model/button
.PATH: ${.CURDIR}/model/entities
.PATH: ${.CURDIR}/model/game
.PATH: ${.CURDIR}/model/gameOver
.PATH: ${.CURDIR}/model/gameWon
.PATH: ${.CURDIR}/model/instructions
.PATH: ${.CURDIR}/model/menu
.PATH: ${.CURDIR}/model/multiPlayer
.PATH: ${.CURDIR}/model/time
.PATH: ${.CURDIR}/view

SRCS = proj.c graphics_card.c player.c monster.c Level.c game.c timer.c kbd.c mouse.c button.c power.c rtc.c time.c multiPlayer.c serial_port.c queue.c menu.c gameWon.c gameOver.c instructions.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
