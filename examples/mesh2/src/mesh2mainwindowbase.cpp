/****************************************************************************
** Form implementation generated from reading ui file 'src\mesh2mainwindowbase.ui'
**
** Created: Do 17. Apr 18:23:17 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "mesh2mainwindowbase.h"

#include <qvariant.h>
#include <../../../src/qwt_plot3d.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qpushbutton.h>
#include <qwt_slider.h>
#include <qwt_wheel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "mesh2mainwindowbase.ui.h"
static const char* const image0_data[] = { 
"22 22 258 2",
"Qt c None",
".a c #292f13",
"bY c #313521",
"bO c #343727",
"c# c #363c20",
"bF c #37382f",
"bu c #383a31",
".V c #383b2a",
"#P c #393b31",
"## c #393c2e",
"b9 c #394020",
"#p c #3a3c32",
"c. c #3c4125",
".E c #404432",
".# c #40491e",
"bE c #464839",
"bt c #46483c",
"#O c #46493d",
"#C c #47493d",
"#o c #474a3d",
".m c #474b38",
"#. c #484a3e",
"b5 c #484c3c",
".U c #494c3f",
"#n c #515446",
".9 c #525547",
"bX c #525d27",
"bP c #535646",
".T c #535648",
"b8 c #535e26",
".D c #555849",
".8 c #575a4b",
"bs c #575c41",
"#w c #585b4c",
"al c #595c4b",
"#5 c #595d4b",
"#N c #595f40",
".S c #5b5e4f",
"bG c #5c5f4e",
"a# c #5d6248",
"#m c #5d6346",
"#x c #5e6349",
"a8 c #5f644a",
"ba c #5f6549",
"bZ c #5f6a30",
"b4 c #5f6c2f",
".b c #5f6d2c",
"bn c #606646",
".7 c #626949",
"ax c #636a45",
"b6 c #63702a",
".l c #646757",
"#H c #646c42",
".C c #66695b",
"aL c #666e48",
"bN c #667723",
"aU c #676e4c",
"aw c #676f45",
"a9 c #687048",
".R c #696d5c",
"aM c #697243",
"am c #69743e",
"bQ c #697635",
"aT c #6b734b",
"aa c #6c783c",
"aD c #6d754f",
"#v c #6d7a3a",
"bh c #6d7c30",
"#1 c #6f7f2f",
"#T c #707e37",
"b0 c #708326",
"a4 c #72822d",
"b3 c #738723",
"bT c #74852a",
".B c #75776b",
"bW c #758a1d",
"bx c #768634",
"bz c #768636",
"b7 c #768c1c",
"ak c #77863b",
"by c #788837",
"#X c #79873f",
"#B c #7a8c2e",
"#K c #7a8c30",
".A c #7b7d71",
".Q c #7b8845",
"#J c #7b8847",
"b# c #7b893f",
".z c #7c7e73",
"ar c #7d855e",
".G c #7f8276",
"#y c #7f9136",
"bH c #809529",
"bD c #81991b",
".k c #828479",
"bi c #828866",
".y c #83886b",
"ab c #839b20",
"af c #848a6a",
"bv c #859933",
"bo c #869740",
"bS c #86a01c",
".q c #888b80",
".c c #898c7e",
"#I c #89974d",
"#g c #89a223",
"bR c #89a41d",
"b2 c #89a517",
".j c #8a8d82",
"#h c #8aa323",
"aP c #8aa51a",
"b1 c #8aa617",
".n c #8b8e80",
".r c #8c8e80",
".x c #8c9566",
"#Y c #8ca521",
"bA c #8ca621",
"#f c #8ca720",
"bV c #8cab13",
".i c #8d8f85",
"#i c #8da529",
".h c #8e9086",
"#l c #8ea91d",
".g c #8f9187",
".6 c #8fa921",
"br c #8fac17",
"bM c #8faf0f",
"a2 c #90a925",
"bC c #90b011",
".f c #92948a",
".d c #939589",
"ae c #93af1d",
"#G c #93b119",
".F c #94968d",
"bb c #94aa39",
"#4 c #94b01c",
"bL c #94b30f",
"bU c #94b311",
"az c #94b313",
"#M c #94b316",
"#u c #94b318",
"ay c #95b319",
"ao c #95b512",
".p c #97998f",
"a7 c #97ac41",
"bK c #97b80f",
".H c #989b89",
"#U c #98a26d",
"aq c #98b328",
"bw c #98b817",
"bJ c #98b90f",
"bg c #98b910",
".s c #999d89",
"#q c #999d8b",
"#6 c #99a17a",
".P c #99b138",
"bB c #99ba0f",
"bI c #99bb0f",
".e c #9a9c93",
"bm c #9bb921",
"a3 c #9bbc0f",
"aO c #9bbd0f",
"#0 c #9cbe0f",
".W c #9da08c",
"bf c #9dbf10",
"an c #9dc011",
"#a c #9ea28e",
"#A c #9ec010",
"#Z c #9ec110",
"#S c #9ec112",
"a5 c #9faf5a",
"b. c #9fb838",
"#L c #9fc011",
"aN c #9fc210",
"#e c #a0c210",
"bq c #a0c310",
"#j c #a1bd31",
"#z c #a1c410",
".5 c #a1c411",
"aC c #a3b945",
"bj c #a4c229",
"aK c #a5c038",
"a1 c #a6c03d",
"aY c #a7c03c",
"#t c #a7c71e",
"bl c #a7c81f",
"bk c #a8c923",
"a. c #a9c829",
"bp c #aaca27",
".o c #abada5",
".w c #abbb65",
"av c #abc738",
"#k c #abca2a",
"#D c #acb294",
"be c #accb2d",
".1 c #adcb2e",
".4 c #adcc2f",
"bc c #afca42",
".2 c #afcd33",
"#d c #afcd34",
"aZ c #afcd35",
".O c #b0ca45",
"#F c #b0cd35",
"aJ c #b0cd36",
"aV c #b3c95b",
"aj c #b3cd45",
".3 c #b3d03f",
".0 c #b4d041",
"aS c #b5cc52",
"aI c #b6d144",
"bd c #b6d145",
"a0 c #b6d146",
".v c #b7c677",
"aX c #b7d14d",
"a6 c #b7d249",
"ad c #b8d24a",
"aQ c #b9cd65",
"#3 c #bad450",
".N c #bdd556",
"#9 c #bdd657",
"#W c #bdd658",
".t c #bfc79a",
".u c #bfcb8b",
"aA c #bfd274",
"aW c #c0d75f",
"au c #c0d860",
"aR c #c1d861",
"aH c #c1d862",
"aB c #c2d964",
".M c #c3d965",
".Z c #c4da6a",
".I c #c5cda9",
"#R c #c5db6c",
"#Q c #cad59a",
".L c #cadd77",
"ai c #cade79",
"ap c #cbde7b",
"aE c #cbde7d",
"#V c #ccdf7d",
"aG c #ccdf7f",
"#2 c #cddc8d",
"#s c #cde080",
"ac c #cee081",
"#c c #cee083",
"#8 c #d1e289",
"aF c #d2e38c",
"at c #d2e38d",
".K c #d4e492",
"as c #d5e2a2",
"#E c #d5e594",
"ag c #d7e0af",
".J c #d7e3a9",
".Y c #d7e69a",
"ah c #dce9a6",
"#7 c #dde9a8",
"#r c #e1eab8",
"#b c #e1ebb8",
".X c #e5eebf",
"QtQtQt.#.a.a.a.a.a.a.a.a.a.a.a.a.a.a.#QtQtQt",
"Qt.b.a.c.d.e.f.g.h.h.h.h.h.h.i.j.k.l.m.a.bQt",
"Qt.a.n.o.p.q.r.s.t.u.v.w.x.y.z.A.B.C.D.E.aQt",
".#.c.o.F.G.H.I.J.K.L.M.N.O.P.Q.R.C.S.T.U.V.#",
".a.d.p.G.W.X.Y.Z.0.1.2.3.3.4.5.6.7.8.9#.##.a",
".a.e.q#a#b#c#d#e#f#g#h#i#j#k.5#e#l#m#n#o#p.a",
".a.f#q#r#s#t#u#v#w#w#w#w#x#y#z#e#A#B#n#C#p.a",
".a.g#D#E#F#G#H#w#w#w#w#I#J#w#K#L#A#M#N#O#P.a",
".a.h#Q#R#S#T#w#w#w#w#U#V#W#X#w#Y#Z#0#1#O#P.a",
".a.h#2#3#4#5#w#w#w#6#7#8#9a.a#aa#Z#0ab#O#P.a",
".a.hacadae#w#w#wafagahaiajakalaman#0ao#O#P.a",
".a.hap#9aq#w#warasahatauavaw#waxay#0az#O#P.a",
".a.haAaBaC#waDaEaFaGaHaIaJaKaLaMaNaOaP#O#P.a",
".a.haQaRaSaTaUaVaWaXaYaZ.3a0a1a2#Za3a4#O#P.a",
".a.ia5a6a0a7a8a9b.b#babbbcbdbe#ebfbgbh#O#P.a",
".a.jbibjbkblbmbn#w#w#w#wbobpbq#Aa3brbsbtbu.a",
".a.k.Bbv#e#e#ebw#KbxbybzbAaNbfaObBbCbDbEbF.a",
".a.l.CbGbH#A#A#A#ZaN#Z#ZbfaObIbJbKbLbMbNbO.a",
".#.m.D.TbPbQbR#0#0#0aOa3bIbJbSbTbUbVbWbXbY.#",
"Qt.a.E.U#.#o#CbZb0b1b1b2b3b4b5btb6b7b8b9.aQt",
"Qt.b.a.V###p#p#P#P#P#P#P#P#P#PbubFc.c#.a.#Qt",
"QtQtQt.#.a.a.a.a.a.a.a.a.a.a.a.a.a.a.#QtQtQt"};

static const char* const image1_data[] = { 
"16 13 5 1",
". c None",
"# c #040404",
"c c #808304",
"a c #f3f704",
"b c #f3f7f3",
".........###....",
"........#...#.#.",
".............##.",
".###........###.",
"#aba#######.....",
"#babababab#.....",
"#ababababa#.....",
"#baba###########",
"#aba#ccccccccc#.",
"#ba#ccccccccc#..",
"#a#ccccccccc#...",
"##ccccccccc#....",
"###########....."};

static const char* const image2_data[] = { 
"21 21 6 1",
". c None",
"a c #0410b0",
"b c #0914b1",
"c c #0b17b2",
"d c #101cb4",
"# c #131eb5",
".....................",
"......#aaaaaaaaaaaab#",
".....a.............ac",
"....a.............a.a",
"...a.............a..a",
"..a.............a...a",
".adaaaaaaaaaaaaa....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a....a",
".a.............a...a.",
".a.............a..a..",
".a.............a.a...",
".a.............aa....",
".aaaaaaaaaaaaaaa.....",
"....................."};

static const char* const image3_data[] = { 
"21 21 2 1",
". c None",
"# c #0410b0",
".....................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#.................",
"...#...............#.",
"...#..............#..",
"....##...........#...",
"......##........#....",
"........##.....#.....",
"..........##..#......",
"............##.......",
".....................",
"....................."};

static const char* const image4_data[] = { 
"21 21 5 1",
". c None",
"# c #0410b0",
"a c #0914b1",
"b c #0b17b2",
"c c #101cb4",
".....................",
".......#.#.#.#.#.#.a.",
".....#..............b",
"..................#..",
"...#................#",
"................#....",
"..c.#.#.#.#.#.#.....#",
".#.............#.....",
"....................#",
".#.............#.....",
"....................#",
".#.............#.....",
"....................#",
".#.............#.....",
"....................#",
".#.............#.....",
"..................#..",
".#.............#.....",
"................#....",
".#.#.#.#.#.#.#.#.....",
"....................."};

static const char* const image5_data[] = { 
"32 32 51 1",
". c None",
"# c #000000",
"p c #734a00",
"w c #744d00",
"q c #755000",
"z c #765100",
"A c #765300",
"o c #775300",
"S c #775400",
"y c #775500",
"r c #785600",
"B c #785700",
"E c #795900",
"R c #795b00",
"N c #7a5b00",
"U c #7a5c00",
"s c #7b5d00",
"l c #7c5f00",
"P c #7f6500",
"k c #7f6600",
"K c #7f6700",
"x c #808080",
"j c #826d00",
"i c #826e00",
"n c #836e00",
"h c #836f00",
"D c #84630a",
"g c #847000",
"f c #847200",
"G c #85640b",
"H c #857400",
"J c #86650c",
"M c #877700",
"d c #8a7e00",
"F c #8c8100",
"c c #8e8600",
"Q c #8f6f13",
"T c #8f7013",
"V c #907114",
"e c #908a00",
"b c #928e00",
"m c #966200",
"a c #969600",
"C c #9d7e1d",
"O c #a08120",
"v c #aa8c27",
"L c #b2952e",
"u c #b69931",
"I c #c4a83c",
"t c #d0b545",
"W c #fffbf3",
"................................",
"................................",
"................................",
"................................",
"................................",
"...######...............######..",
"..#.#.#.###...........###.#.#.#.",
".#.######.###.......##.#######.#",
".####abcd##.#.....##.###aeddf###",
".##e#ghijkl##...##.###mn####op##",
".##o#pqrsrqp#.##.###tt######op##",
".##o#pqrsrqp##.###ttuv######w#x#",
".##y#zAyB###x####tuuCD##x####x##",
".#x##Er###x###tt#uCCDD#F#x#x####",
".#######x###ttuu#CDDDG#HI####te#",
".##x#x#x##ttuuCC#DDDGJ#KLttttuM#",
".#e#####e#uuCCDD#DGGJy#NOuuuuCP#",
".#MtttttM#CCDDDD#GJJ###NQCCCCDo#",
".#PutttuP#DDDDGG#J##.##RQDDDDDS#",
".#oCvvvCo#DDGGJJ##.####RTDDDDGS#",
".#SDDDDDo#GGJJ##.###..#UVGGGGJy#",
".#SDDDDDS#JJ##.###....###JJJJ###",
".#yGGGGGS###.###.......#.#####.#",
".##JJJJJy#.###..........##...##.",
".#.#########.............#####..",
"..##.#.#.#......................",
"...#######......................",
"..........................W.....",
"................................",
"................................",
"................................",
"................................"};

static const char* const image6_data[] = { 
"16 16 48 1",
". c None",
"N c #062dff",
"Q c #082eff",
"T c #1b3ff1",
"S c #2651fb",
"O c #2b59fe",
"r c #2f50e1",
"k c #3154e2",
"E c #375ef3",
"B c #3a65ff",
"K c #3c65ee",
"w c #3d4ebb",
"J c #445bc2",
"z c #4775ff",
"R c #4860c5",
"v c #4d7dff",
"A c #506fcc",
"I c #537ceb",
"q c #6597ff",
"m c #6b92e2",
"C c #6c84ba",
"P c #6c9dff",
"L c #6ca1ff",
"s c #6e87c7",
"H c #76a6fe",
"f c #7ea2ea",
"D c #7fafff",
"d c #82a9e8",
"y c #87b7ff",
"M c #87b8ff",
"x c #8e9ec0",
"u c #8ebeff",
"e c #8f99b7",
"p c #97c8ff",
"t c #9dceff",
"# c #9fa2b3",
"o c #9fd3ff",
"F c #a2cef6",
"j c #a2d1ff",
"l c #a4b4cd",
"a c #a6baec",
"c c #b2c9ee",
"n c #b5ebff",
"b c #b9caee",
"G c #bad6f8",
"i c #dafaff",
"h c #e1faff",
"g c #f6ffff",
".....#abcde.....",
".....fghijk#....",
"....lmnopqr.....",
".....sptuvwl....",
".....xypyzA.....",
".....eypyBC.....",
".....lfpDE#.....",
".....ldtDE#.....",
".l...eFtyvCl..l.",
".sFGayuyyyHffIJ.",
".CKLMMyyDyDHBNA.",
"..COPyyyDDHzQR#.",
"...COPDDHHvNwl..",
"....CSqHqzNw#...",
".....CSvvQwl....",
"......srTA#....."};

static const char* const image7_data[] = { 
"21 21 167 2",
"Qt c None",
"#Y c #0c0c0c",
"ai c #0d0d0d",
".C c #0e0e0e",
".5 c #0f0f0f",
"#Q c #101010",
"#d c #121212",
".f c #131313",
".n c #141414",
".w c #151515",
"#i c #161616",
".g c #171717",
".h c #181818",
".M c #18181b",
".v c #1a1a1a",
"#F c #1b1b1b",
".c c #1b1b26",
".L c #1c1c1c",
"#G c #1e1e1e",
".b c #1f1f1f",
"aj c #202020",
"#B c #212121",
".V c #222222",
".9 c #242424",
".Q c #24242f",
"#p c #262626",
".U c #272727",
"aF c #282828",
".a c #282869",
"az c #292929",
"aK c #292945",
".T c #2a2a2a",
".K c #2b2b2b",
".1 c #2b2b5f",
"av c #2c2c33",
"#L c #2e2e2e",
"aJ c #2e2e4e",
"aG c #303030",
".P c #303031",
"ah c #313131",
".S c #33336c",
"#u c #343434",
"#A c #363636",
"## c #363661",
"#E c #37375d",
"#5 c #3a3a3a",
".u c #3b3b3b",
".R c #3b3baa",
"#3 c #3c3c3c",
"aH c #3d3d3d",
".D c #3e3e73",
".d c #4040db",
"ak c #41414b",
"#H c #424242",
".k c #4343c0",
".y c #444444",
".# c #4545e2",
".e c #474799",
"#S c #484848",
"aE c #494949",
".E c #494994",
"#I c #4b4b5f",
".r c #4d4da7",
"#r c #4d4ded",
"#e c #4e4e4f",
".0 c #4f4f52",
"#D c #4f4ff7",
"aC c #5050e1",
"#q c #525260",
"#J c #5252f1",
".H c #5252fe",
".Z c #555555",
".7 c #5656ff",
"#l c #575787",
".A c #5757ff",
"au c #5a5a96",
"aD c #5a5aab",
"al c #5a5ae5",
"#U c #5a5af1",
".i c #5b5b5b",
"#K c #5b5b71",
".X c #5b5bff",
"#x c #5e5eff",
"aI c #5f5f5f",
"#. c #606061",
"#k c #616161",
"#2 c #636363",
".O c #6565d9",
".o c #676767",
".F c #68686b",
"aw c #696969",
"#T c #696970",
".W c #69697d",
".s c #6969fe",
".N c #6a6aed",
"ay c #6e6e6e",
"#t c #6f6f6f",
"aB c #72727c",
"af c #7272ff",
"a# c #7474ff",
".z c #7575af",
"#0 c #7575ff",
"#R c #777777",
"#O c #7777ff",
"#w c #79799e",
"#m c #7a7aff",
"#X c #7b7b7b",
"#g c #7d7dff",
".l c #8080f6",
"#s c #8181d0",
"#7 c #8484ff",
"#9 c #878787",
"#f c #8787fc",
"ab c #8a8a8a",
"as c #8e8e8e",
"#M c #919191",
"aA c #949494",
"#V c #9595c6",
".q c #969696",
"ax c #999999",
"#a c #9a9aff",
".m c #9d9d9d",
"#W c #9e9e9e",
".4 c #9f9f9f",
".2 c #9f9fe8",
"an c #a0a0a0",
"am c #a1a1e2",
".G c #a3a3c6",
".6 c #a7a7e3",
"at c #a8a8b3",
"#P c #a9a9b0",
".j c #adadad",
".J c #aeaeae",
"#h c #b0b0b3",
"ae c #b5b5ff",
".I c #b9b9db",
"#C c #babae2",
".Y c #babafe",
"#N c #babaff",
"#6 c #bcbcbd",
"#1 c #bdbdfe",
"a. c #bebeff",
".x c #cccccc",
"#j c #cdcdcd",
"ao c #cecece",
".8 c #cfcfea",
"ad c #d1d1d1",
"#c c #d4d4d4",
"#8 c #d7d7d7",
"aq c #d9d9ff",
"#Z c #dbdbe6",
"aa c #dddde7",
"ac c #dedede",
"ap c #dfdfff",
".3 c #e6e6e6",
".B c #e6e6ff",
"#z c #e8e8e8",
".t c #e9e9f2",
"#v c #eaeaea",
"#y c #ededfb",
"#o c #f1f1f1",
"#4 c #f2f2f2",
".p c #f5f5f5",
"ag c #f8f8ff",
"ar c #fbfbfb",
"#n c #fcfcff",
"#b c #ffffff",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQt.b.c.d.e.f.gQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.h.i.j.k.l.m.fQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.n.o.p.q.r.s.t.u.vQtQtQtQt",
"QtQtQtQtQtQtQtQt.w.x.p.y.z.A.B.m.CQtQtQtQt",
"QtQtQtQtQtQtQt.f.D.E.F.v.G.H.I.J.K.LQtQtQt",
"QtQtQtQtQtQtQt.M.N.O.P.f.Q.R.S.T.U.VQtQtQt",
"QtQtQtQtQtQt.n.W.X.Y.Z.i.0.1.2.3.4.5QtQtQt",
"QtQtQtQtQtQt.w.6.7.8.9.m#.###a#b#c#dQtQtQt",
"QtQtQtQtQt.n#e#f#g#h#i#j#k#l#m#n#o#pQtQtQt",
"QtQtQtQtQt.C#q#r#s#t#u#v#k#w#x#y#z#A#BQtQt",
"QtQtQtQt.w#A#C#D#E#F#G#H.u#I#J#K#L#B.fQtQt",
"QtQtQtQt.C#M#N#O#P#Q#R.m#S#T#U#V#W#X#YQtQt",
"QtQtQtQt.9#Z#0#1#2#3#o#4#5#6#7#a#b#8.wQtQt",
"QtQtQt.5#9a.a#aa.9ab#bac.Uadaeafag#4ah.bQt",
"QtQtaiajakalaman#i#j#baoaj#8ap.Xaqar#MaiQt",
"Qt.w.gasatauavaj.9awaxayazaAaBaCaDaEaFajaG",
"QtaHQtQtQtQtQtaI#A.K.K.U.9.b.baJaK#BQtQt.C",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image8_data[] = { 
"21 21 105 2",
"Qt c None",
"#v c #0c0c0c",
".d c #0d0d0d",
".B c #0e0e0e",
".X c #0f0f0f",
".# c #101010",
".a c #111111",
".4 c #121212",
".f c #131313",
".m c #141414",
".v c #151515",
".7 c #161616",
".g c #171717",
".c c #181818",
".j c #191919",
".u c #1a1a1a",
"#l c #1b1b1b",
".I c #1c1c1c",
".z c #1e1e1e",
".b c #1f1f1f",
"#D c #202020",
"#i c #212121",
".P c #222222",
"#M c #232323",
".0 c #242424",
"## c #262626",
".O c #272727",
".M c #282828",
"#L c #292929",
".N c #2a2a2a",
".H c #2b2b2b",
".e c #2c2c2c",
"#o c #2e2e2e",
".q c #2f2f2f",
".L c #303030",
"#C c #313131",
"#e c #343434",
"#h c #363636",
".r c #373737",
".C c #393939",
".t c #3b3b3b",
"#y c #3c3c3c",
"#J c #3d3d3d",
"#H c #3e3e3e",
"#E c #414141",
".D c #424242",
".x c #444444",
"#I c #494949",
"#n c #4c4c4c",
".2 c #4d4d4d",
".5 c #4e4e4e",
"#b c #515151",
"#a c #535353",
".S c #555555",
".k c #595959",
".h c #5b5b5b",
"#K c #5f5f5f",
"#m c #616161",
".y c #626262",
"#x c #636363",
".n c #676767",
".E c #696969",
".9 c #6c6c6c",
".Q c #6e6e6e",
"#d c #6f6f6f",
"#q c #777777",
".3 c #787878",
"#u c #7b7b7b",
"#t c #868686",
".U c #878787",
"#A c #8a8a8a",
"#k c #8e8e8e",
".K c #919191",
"#s c #929292",
".p c #969696",
".F c #989898",
"#G c #999999",
".l c #9d9d9d",
"#r c #9e9e9e",
".W c #9f9f9f",
"#F c #a0a0a0",
"#c c #a8a8a8",
".i c #acacac",
"#p c #adadad",
".G c #aeaeae",
".6 c #b2b2b2",
".J c #bebebe",
".T c #c6c6c6",
".w c #cccccc",
".8 c #cdcdcd",
".Y c #d4d4d4",
"#z c #d7d7d7",
"#j c #d8d8d8",
".A c #dedede",
"#w c #e4e4e4",
".V c #e6e6e6",
".s c #e7e7e7",
"#g c #e8e8e8",
"#f c #eaeaea",
"#. c #f1f1f1",
"#B c #f2f2f2",
".o c #f5f5f5",
".1 c #fbfbfb",
".R c #fefefe",
".Z c #ffffff",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQt.b.c.d.e.f.gQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.c.h.i.j.k.l.fQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.m.n.o.p.q.r.s.t.uQtQtQtQt",
"QtQtQtQtQtQtQtQt.v.w.o.x.y.z.A.l.BQtQtQtQt",
"QtQtQtQtQtQtQt.f.C.D.E.u.F.j.G.G.H.IQtQtQt",
"QtQtQtQtQtQtQt.g.J.K.L.f.M.a.b.N.O.PQtQtQt",
"QtQtQtQtQtQt.m.Q.R.R.S.h.T.M.U.V.W.XQtQtQt",
"QtQtQtQtQtQt.v.Y.Z.V.0.l.1.2.3.Z.Y.4QtQtQt",
"QtQtQtQtQt.m.5.o.Z.6.7.8.Z.9.2.1#.##QtQtQt",
"QtQtQtQtQt.B#a#b#c#d#e#f.Z.l.M#g#g#h#iQtQt",
"QtQtQtQt.v#h#j#k#e#l.z.D#m#n.I#a#o#i.fQtQt",
"QtQtQtQt.B.K.Z.Z#p.##q#r#r#s.O#t#r#u#vQtQt",
"QtQtQtQt.0#w.Z.R#x#y#..Z.Z.R.h.3.Z#z.vQtQt",
"QtQtQt.X.U.Z.Z.V.0#A.Z.Z.Z.Z.l.D.o#B#C.bQt",
"QtQt.d#D#E#A.8#F.7.8.Z.Z.Z.Z.Y.0.8.1.K.dQt",
"Qt.v.g#k.G.k.N#D.0.E#G#r#r.l#d#D#H#I.M#D.L",
"Qt#JQtQtQtQtQt#K#h.H.H.H#L.b.b#M.b#iQtQt.B",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image9_data[] = { 
"21 21 62 1",
". c None",
"P c #0d0d0d",
"q c #0e0e0e",
"y c #0f0f0f",
"A c #121212",
"f c #131313",
"k c #141414",
"w c #151515",
"a c #161616",
"g c #171717",
"h c #181818",
"o c #1a1a1a",
"# c #1b1b1b",
"c c #1c1c1c",
"J c #1d1d1d",
"t c #1e1e1e",
"b c #1f1f1f",
"Q c #202020",
"7 c #212121",
"L c #242424",
"D c #262626",
"2 c #282828",
"G c #292929",
"V c #2b2b2b",
"3 c #303030",
"O c #313131",
"F c #363636",
"n c #3b3b3b",
"4 c #3d3d3d",
"6 c #464646",
"d c #474747",
"W c #494949",
"e c #4f4f4f",
"B c #555555",
"U c #595959",
"i c #5b5b5b",
"1 c #5e5e5e",
"v c #606060",
"X c #616161",
"l c #676767",
"I c #686868",
"Y c #6c6c6c",
"r c #6e6e6e",
"0 c #757575",
"5 c #898989",
"S c #8e8e8e",
"T c #aeaeae",
"Z c #cb0000",
"R c #df0000",
"N c #f20000",
"M c #ff0603",
"K c #ff1a0d",
"H c #ff2f17",
"E c #ff4220",
"C c #ff5429",
"z c #ff6632",
"x c #ff793b",
"u c #ff8a3d",
"s c #ff9b36",
"p c #ffac30",
"m c #ffbd29",
"j c #ffcd22",
".....................",
"............#a.......",
"..........bcdefg.....",
".........hijjjjf.....",
"........klmmmmmno....",
"........gpppppppq....",
".......krssssssst....",
".......guuuuuuuuvw...",
"......krxxxxxxxxxy...",
"......wzzzzzzzzzzA...",
".....kBCCCCCCCCCCD...",
".....qEEEEEEEEEEEFG..",
"....wFHHHHHHHHHHHIJ..",
"....qKKKKKKKKKKKKKy..",
"....LMMMMMMMMMMMMMw..",
"...yNNNNNNNNNNNNNNOb.",
"..PQRRRRRRRRRRRRRRRP.",
".wgSTUVWXYZZZZ011W2Q3",
".4.....56VVVGbbbb7..q",
".....................",
"....................."};

static const char* const image10_data[] = { 
"21 21 118 2",
"Qt c None",
"#q c #0c0c0c",
".d c #0d0d0d",
"#H c #0e0d0d",
".3 c #0e0e0e",
".t c #0f0e0e",
".S c #100f0f",
".# c #101010",
".a c #111111",
".u c #131313",
".N c #141414",
".f c #151313",
"#a c #151515",
".X c #161212",
".k c #161313",
".p c #171515",
".g c #171717",
"#w c #181515",
"#m c #190d0d",
"#o c #1a0f0f",
".c c #1a1818",
".y c #1a1a1a",
"#x c #1b0e0e",
".z c #1b1919",
".o c #1b1a1a",
"#e c #1b1b1b",
".D c #1c1c1c",
"#j c #1d1c1c",
".H c #1e1212",
".T c #1f1414",
".J c #1f1f1f",
".l c #201313",
"#C c #201f1f",
"#M c #202020",
"#F c #211515",
".E c #211616",
"#P c #212020",
"#l c #212121",
"#D c #222020",
"## c #222121",
".M c #222222",
".0 c #231414",
".j c #231717",
"#Z c #232323",
"#z c #252424",
"#N c #262424",
"#G c #272323",
"#f c #281c1c",
"#S c #282828",
".s c #291c1c",
".L c #292727",
".R c #292828",
"#Y c #292929",
".I c #2a2828",
"#L c #2a2a2a",
".b c #2b1d1d",
".9 c #2b2727",
"#X c #2b2b2b",
".C c #2c2b2b",
".h c #2d1515",
"#r c #2f2121",
"#k c #302d2d",
"#T c #303030",
".V c #312121",
"#p c #312525",
".n c #322e2e",
".K c #332828",
".e c #352929",
"#d c #353434",
".8 c #363333",
"#W c #363636",
"#U c #3d3d3d",
"#Q c #3e3e3e",
".v c #413636",
"#u c #453838",
"#b c #463030",
"#R c #494949",
".w c #4b3d3d",
"#A c #4d3c3c",
".Y c #4f4d4d",
".W c #514b4b",
".5 c #525050",
".1 c #564747",
"#g c #583636",
".4 c #594e4e",
"#K c #595959",
"#v c #5e5959",
"#V c #5f5f5f",
".r c #626262",
"#t c #646262",
".2 c #6b1c1c",
"#. c #6e2a2a",
".G c #6f2222",
".7 c #6f6f6f",
"#B c #712222",
".O c #7d5a5a",
"#i c #7e3232",
".Q c #853939",
"#I c #8e8e8e",
"#h c #a62727",
"#J c #aeaeae",
".A c #b77070",
".B c #b77474",
"#O c #cb0000",
"#E c #df0000",
"#y c #f20000",
"#s c #ff0603",
"#n c #ff1a0d",
"#c c #ff2f17",
".6 c #ff4220",
".Z c #ff5429",
".U c #ff6632",
".P c #ff793b",
".F c #ff8a3d",
".x c #ff9b36",
".q c #ffac30",
".m c #ffbd29",
".i c #ffcd22",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQt.b.c.d.e.f.gQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.h.i.i.j.i.i.kQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.l.m.m.m.n.m.m.m.oQtQtQtQt",
"QtQtQtQtQtQtQtQt.p.q.q.q.r.s.q.q.tQtQtQtQt",
"QtQtQtQtQtQtQt.u.v.w.x.y.x.z.A.B.C.DQtQtQt",
"QtQtQtQtQtQtQt.E.F.F.G.H.I.a.J.K.L.MQtQtQt",
"QtQtQtQtQtQt.N.O.P.P.Q.P.P.R.P.P.P.SQtQtQt",
"QtQtQtQtQtQt.T.U.U.U.V.U.U.W.U.U.U.XQtQtQt",
"QtQtQtQtQt.N.Y.Z.Z.Z.0.Z.Z.Z.1.Z.Z.2QtQtQt",
"QtQtQtQtQt.3.4.5.6.7.8.6.6.6.9.6.6#.##QtQt",
"QtQtQtQt#a#b#c#c#d#e#f#g#h#i#j.4#k#l.uQtQt",
"QtQtQtQt#m#n#n#n#n#o#n#n#n#n#p#n#n#n#qQtQt",
"QtQtQtQt#r#s#s#s#t#u#s#s#s#s#v#s#s#s#wQtQt",
"QtQtQt#x#y#y#y#y#z#y#y#y#y#y#y#A#y#y#B#CQt",
"QtQt.d#D#E#E#E#E#F#E#E#E#E#E#E#G#E#E#E#HQt",
"Qt#a.g#I#J#K#L#M#N#O#O#O#O#O#O#P#Q#R#S#M#T",
"Qt#UQtQtQtQtQt#V#W#X#X#X#Y.J.J#Z.J#lQtQt.3",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image11_data[] = { 
"21 21 77 2",
"Qt c None",
"#g c #050505",
".# c #0a0a0a",
"#k c #0e0e0e",
".a c #0f0f0f",
".G c #111111",
".y c #121212",
".q c #1a1a1a",
"#. c #1e1e1e",
".F c #1f1f1f",
".c c #202020",
".n c #232323",
".b c #2a2a2a",
".N c #2b2b2b",
".M c #2d2d2d",
".L c #2e2e2e",
".0 c #313131",
"#j c #373737",
".C c #3a3a3a",
".8 c #3b3b3b",
".d c #3c3c3c",
".i c #424242",
"#a c #4b4b4b",
".z c #4c4c4c",
"#h c #4d4d4d",
".U c #4e4e4e",
".u c #5d5d5d",
".X c #696969",
".2 c #6c6c6c",
".m c #6d6d6d",
".H c #717171",
".h c #747474",
"## c #787878",
"#i c #7d7d7d",
".T c #989898",
".t c #999999",
".K c #9a9a9a",
"#b c #9d9d9d",
".p c #9e9e9e",
".V c #9f9f9f",
".r c #a1a1a1",
"#c c #a2a2a2",
".1 c #a4a4a4",
".v c #a6a6a6",
".x c #aeaeae",
".I c #b0b0b0",
"#f c #b2b2b2",
".R c #b4b4b4",
".4 c #b8b8b8",
".O c #bdbdbd",
".D c #c0c0c0",
".W c #c3c3c3",
".7 c #cfcfcf",
"#e c #d8d8d8",
"#d c #dadada",
".9 c #dbdbdb",
".6 c #dedede",
".E c #e0e0e0",
".3 c #e3e3e3",
".B c #e4e4e4",
".e c #e5e5e5",
".o c #e6e6e6",
".A c #e9e9e9",
".5 c #ebebeb",
".w c #eeeeee",
".l c #f1f1f1",
".Q c #f2f2f2",
".g c #f3f3f3",
".Y c #f4f4f4",
".j c #f7f7f7",
".s c #f8f8f8",
".J c #f9f9f9",
".Z c #fafafa",
".S c #fbfbfb",
".P c #fcfcfc",
".f c #fefefe",
".k c #ffffff",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQt.b.c.d.eQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQt.f.g.h.i.j.kQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.k.k.l.m.n.o.p.qQtQtQtQt",
"QtQtQtQtQtQtQtQt.r.s.k.t.u.v.w.x.yQtQtQtQt",
"QtQtQtQtQtQtQtQt.z.A.B.C.D.k.E.t.F.GQtQtQt",
"QtQtQtQtQtQtQt.H.l.k.B.I.J.k.K.L.M.NQtQtQt",
"QtQtQtQtQtQtQt.O.k.k.k.k.k.k.P.Q.jQtQtQtQt",
"QtQtQtQtQtQt.k.k.k.k.k.k.k.k.k.k.k.kQtQtQt",
"QtQtQtQtQtQt.R.s.k.S.f.k.k.k.s.k.k.kQtQtQt",
"QtQtQtQtQt.T.U.A.k.t.V.k.k.W.X.Y.k.kQtQtQt",
"QtQtQtQtQt.Z.Y.k.A.0.1.k.j.2.T.S.k.kQtQtQt",
"QtQtQtQt.i.3.k.k.s.4.5.k.S.I.6.k.k.7.8QtQt",
"QtQtQtQt.T.w.k.k.k.k.k.k.k.k.k.k.k.9#.QtQt",
"QtQtQt##.s.k.k.k.k.k.k.k.k.k.k.k.k.S#aQtQt",
"QtQtQt.i.A.k.k.k.k.k.k.k.k.k.k.k.k.k.7#bQt",
"QtQt.s.Y.k.k.f.V.7.k.w#c#d.k.k#e#f.k.k.kQt",
"#gQtQtQtQtQtQt#h#iQtQt.0.2QtQtQt#jQtQtQt#k",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image12_data[] = { 
"21 21 10 1",
". c None",
"# c #000000",
"a c #304d5f",
"b c #355659",
"c c #395f52",
"d c #3e684c",
"e c #437145",
"f c #487a3e",
"g c #4d8337",
"h c #528c2f",
".....................",
".....................",
".....................",
".....................",
".....................",
".....................",
"....################.",
"....#aaaaaaaaaaaaaa#.",
"...#bbbbbbbbbbbbbbb#.",
"...#cccccccccccccc#..",
"..#ddddddddddddddd#..",
"..#eeeeeeeeeeeeee#...",
".#fffffffffffffff#...",
".#gggggggggggggg#....",
"#hhhhhhhhhhhhhhh#....",
"################.....",
".....................",
".....................",
".....................",
".....................",
"....................."};

static const char* const image13_data[] = { 
"21 21 4 1",
". c None",
"# c #000000",
"b c #0111dd",
"a c #d9090e",
".....................",
".....................",
".....................",
".....................",
".....................",
".....................",
"....################.",
"....#....a....a....#.",
"...#..aaa.....a....#.",
"...#aa....bb...aa.#..",
"..#.....bb..b....a#..",
"..#....b....b....#...",
".#.aa..b..bb.aaaa#...",
".#...a..bb..a...#....",
"#....a.....a....#....",
"################.....",
".....................",
".....................",
".....................",
".....................",
"....................."};

static const char* const image14_data[] = { 
"21 21 3 1",
". c None",
"# c #000000",
"a c #013add",
".....................",
".....................",
".....................",
".....................",
".....................",
".....................",
"....################.",
"....#...a...a...a..#.",
"...#...a...a...a...#.",
"...#aaaaaaaaaaaaaa#..",
"..#...a...a...a...#..",
"..#...a...a...a..#...",
".#aaaaaaaaaaaaaaa#...",
".#...a...a...a..#....",
"#...a...a...a...#....",
"################.....",
".....................",
".....................",
".....................",
".....................",
"....................."};

static const char* const image15_data[] = { 
"21 21 2 1",
". c None",
"# c #000000",
".....................",
".....................",
".....................",
".....................",
".....................",
".....................",
"....################.",
"....#..............#.",
"...#...............#.",
"...#..............#..",
"..#...............#..",
"..#..............#...",
".#...............#...",
".#..............#....",
"#...............#....",
"################.....",
".....................",
".....................",
".....................",
".....................",
"....................."};


/* 
 *  Constructs a Mesh2MainWindowBase which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
Mesh2MainWindowBase::Mesh2MainWindowBase( QWidget* parent,  const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    QPixmap image3( ( const char** ) image3_data );
    QPixmap image4( ( const char** ) image4_data );
    QPixmap image5( ( const char** ) image5_data );
    QPixmap image6( ( const char** ) image6_data );
    QPixmap image7( ( const char** ) image7_data );
    QPixmap image8( ( const char** ) image8_data );
    QPixmap image9( ( const char** ) image9_data );
    QPixmap image10( ( const char** ) image10_data );
    QPixmap image11( ( const char** ) image11_data );
    QPixmap image12( ( const char** ) image12_data );
    QPixmap image13( ( const char** ) image13_data );
    QPixmap image14( ( const char** ) image14_data );
    QPixmap image15( ( const char** ) image15_data );
    if ( !name )
	setName( "Mesh2MainWindowBase" );
    resize( 904, 586 ); 
    setMinimumSize( QSize( 778, 456 ) );
    setCaption( trUtf8( "Mesh2" ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    Mesh2MainWindowBaseLayout = new QGridLayout( centralWidget(), 1, 1, 11, 6, "Mesh2MainWindowBaseLayout"); 

    Frame3 = new QFrame( centralWidget(), "Frame3" );
    Frame3->setFrameShape( QFrame::StyledPanel );
    Frame3->setFrameShadow( QFrame::Sunken );
    Frame3Layout = new QGridLayout( Frame3, 1, 1, 11, 6, "Frame3Layout"); 

    dataWidget = new QwtPlot3D( Frame3, "dataWidget" );

    Frame3Layout->addWidget( dataWidget, 0, 0 );

    Mesh2MainWindowBaseLayout->addMultiCellWidget( Frame3, 0, 0, 1, 3 );

    Layout19 = new QVBoxLayout( 0, 0, 6, "Layout19"); 

    Layout17 = new QVBoxLayout( 0, 0, 6, "Layout17"); 

    TextLabel1 = new QLabel( centralWidget(), "TextLabel1" );
    TextLabel1->setText( trUtf8( "Polygon Offs." ) );
    Layout17->addWidget( TextLabel1 );

    offsetSlider = new QwtSlider( centralWidget(), "offsetSlider" );
    offsetSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)3, 0, 0, offsetSlider->sizePolicy().hasHeightForWidth() ) );
    offsetSlider->setOrientation( QwtSlider::Vertical );
    offsetSlider->setThumbLength( 15 );
    offsetSlider->setThumbWidth( 23 );
    offsetSlider->setBgStyle( QwtSlider::BgSlot );
    offsetSlider->setScalePos( QwtSlider::Left );
    QToolTip::add( offsetSlider, trUtf8( "Paint improvement mesh-polygon boundary" ) );
    QWhatsThis::add( offsetSlider, trUtf8( "Paint improvement mesh-polygon boundary" ) );
    Layout17->addWidget( offsetSlider );
    Layout19->addLayout( Layout17 );
    QSpacerItem* spacer = new QSpacerItem( 0, 85, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout19->addItem( spacer );

    Layout11 = new QGridLayout( 0, 1, 1, 0, 6, "Layout11"); 

    yS = new QwtWheel( centralWidget(), "yS" );
    yS->setMinimumSize( QSize( 0, 100 ) );
    yS->setMaximumSize( QSize( 32767, 100 ) );
    yS->setFocusPolicy( QwtWheel::StrongFocus );
    yS->setOrientation( QwtWheel::Vertical );
    yS->setTickCnt( 20 );

    Layout11->addWidget( yS, 2, 1 );

    ySc = new QwtWheel( centralWidget(), "ySc" );
    ySc->setMinimumSize( QSize( 0, 100 ) );
    ySc->setMaximumSize( QSize( 32767, 100 ) );
    ySc->setFocusPolicy( QwtWheel::StrongFocus );
    ySc->setMass( 0.1 );
    ySc->setOrientation( QwtWheel::Vertical );
    ySc->setTickCnt( 20 );

    Layout11->addWidget( ySc, 2, 0 );

    yR = new QwtWheel( centralWidget(), "yR" );
    yR->setMinimumSize( QSize( 0, 100 ) );
    yR->setMaximumSize( QSize( 32767, 100 ) );
    yR->setCursor( QCursor( 0 ) );
    yR->setFocusPolicy( QwtWheel::StrongFocus );
    yR->setMass( 0.1 );
    yR->setOrientation( QwtWheel::Vertical );
    yR->setTickCnt( 10 );

    Layout11->addWidget( yR, 2, 2 );

    TextLabel1_2 = new QLabel( centralWidget(), "TextLabel1_2" );
    TextLabel1_2->setText( trUtf8( "Y" ) );
    TextLabel1_2->setAlignment( int( QLabel::AlignCenter ) );

    Layout11->addMultiCellWidget( TextLabel1_2, 0, 0, 0, 2 );

    TextLabel2 = new QLabel( centralWidget(), "TextLabel2" );
    TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2->setText( trUtf8( "scale   shift  rot" ) );
    TextLabel2->setAlignment( int( QLabel::AlignCenter ) );

    Layout11->addMultiCellWidget( TextLabel2, 1, 1, 0, 2 );
    Layout19->addLayout( Layout11 );

    Mesh2MainWindowBaseLayout->addLayout( Layout19, 0, 0 );

    Layout21 = new QVBoxLayout( 0, 0, 6, "Layout21"); 

    Layout14 = new QVBoxLayout( 0, 0, 6, "Layout14"); 

    projection = new QCheckBox( centralWidget(), "projection" );
    projection->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, projection->sizePolicy().hasHeightForWidth() ) );
    projection->setText( trUtf8( "Ortho" ) );
    projection->setChecked( TRUE );
    Layout14->addWidget( projection );

    colorlegend = new QCheckBox( centralWidget(), "colorlegend" );
    colorlegend->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, colorlegend->sizePolicy().hasHeightForWidth() ) );
    colorlegend->setText( trUtf8( "Legend" ) );
    Layout14->addWidget( colorlegend );

    autoscale = new QCheckBox( centralWidget(), "autoscale" );
    autoscale->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, autoscale->sizePolicy().hasHeightForWidth() ) );
    autoscale->setText( trUtf8( "Autoscale" ) );
    autoscale->setChecked( TRUE );
    QToolTip::add( autoscale, trUtf8( "autoscale axes" ) );
    QWhatsThis::add( autoscale, trUtf8( "autoscale axes" ) );
    Layout14->addWidget( autoscale );
    Layout21->addLayout( Layout14 );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 130, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout21->addItem( spacer_2 );

    Layout13_2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout13_2"); 

    zR = new QwtWheel( centralWidget(), "zR" );
    zR->setMinimumSize( QSize( 0, 100 ) );
    zR->setMaximumSize( QSize( 32767, 100 ) );
    zR->setCursor( QCursor( 0 ) );
    zR->setFocusPolicy( QwtWheel::StrongFocus );
    zR->setMass( 0.1 );
    zR->setOrientation( QwtWheel::Vertical );
    zR->setTickCnt( 10 );

    Layout13_2->addWidget( zR, 2, 0 );

    TextLabel1_2_2 = new QLabel( centralWidget(), "TextLabel1_2_2" );
    TextLabel1_2_2->setText( trUtf8( "Z" ) );
    TextLabel1_2_2->setAlignment( int( QLabel::AlignCenter ) );

    Layout13_2->addMultiCellWidget( TextLabel1_2_2, 0, 0, 0, 2 );

    TextLabel2_4 = new QLabel( centralWidget(), "TextLabel2_4" );
    TextLabel2_4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2_4->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_4->setText( trUtf8( "rot   shift  scale" ) );
    TextLabel2_4->setAlignment( int( QLabel::AlignCenter ) );

    Layout13_2->addMultiCellWidget( TextLabel2_4, 1, 1, 0, 2 );

    zSc = new QwtWheel( centralWidget(), "zSc" );
    zSc->setMinimumSize( QSize( 0, 100 ) );
    zSc->setMaximumSize( QSize( 32767, 100 ) );
    zSc->setFocusPolicy( QwtWheel::StrongFocus );
    zSc->setMass( 0.1 );
    zSc->setOrientation( QwtWheel::Vertical );
    zSc->setTickCnt( 20 );

    Layout13_2->addWidget( zSc, 2, 2 );

    zS = new QwtWheel( centralWidget(), "zS" );
    zS->setMinimumSize( QSize( 0, 100 ) );
    zS->setMaximumSize( QSize( 32767, 100 ) );
    zS->setFocusPolicy( QwtWheel::StrongFocus );
    zS->setOrientation( QwtWheel::Vertical );
    zS->setTickCnt( 20 );

    Layout13_2->addWidget( zS, 2, 1 );
    Layout21->addLayout( Layout13_2 );
    QSpacerItem* spacer_3 = new QSpacerItem( 16, 130, QSizePolicy::Minimum, QSizePolicy::Preferred );
    Layout21->addItem( spacer_3 );

    Mesh2MainWindowBaseLayout->addMultiCellLayout( Layout21, 0, 1, 4, 4 );

    normButton = new QPushButton( centralWidget(), "normButton" );
    normButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, normButton->sizePolicy().hasHeightForWidth() ) );
    normButton->setMinimumSize( QSize( 50, 35 ) );
    normButton->setMaximumSize( QSize( 50, 35 ) );
    normButton->setCursor( QCursor( 0 ) );
    normButton->setText( trUtf8( "Std" ) );

    Mesh2MainWindowBaseLayout->addWidget( normButton, 1, 0 );

    Layout22 = new QGridLayout( 0, 1, 1, 0, 6, "Layout22"); 

    Layout5_3 = new QHBoxLayout( 0, 0, 6, "Layout5_3"); 

    betaLabel = new QLabel( centralWidget(), "betaLabel" );
    betaLabel->setText( trUtf8( "b" ) );
    betaLabel->setTextFormat( QLabel::RichText );
    betaLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    Layout5_3->addWidget( betaLabel );

    LCDNumber2_3 = new QLCDNumber( centralWidget(), "LCDNumber2_3" );
    LCDNumber2_3->setFrameShape( QLCDNumber::Panel );
    LCDNumber2_3->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2_3->setSmallDecimalPoint( TRUE );
    LCDNumber2_3->setSegmentStyle( QLCDNumber::Flat );
    Layout5_3->addWidget( LCDNumber2_3 );

    Layout22->addLayout( Layout5_3, 2, 1 );

    TextLabel1_4 = new QLabel( centralWidget(), "TextLabel1_4" );
    TextLabel1_4->setText( trUtf8( "Resolution" ) );
    TextLabel1_4->setAlignment( int( QLabel::AlignCenter ) );

    Layout22->addMultiCellWidget( TextLabel1_4, 0, 0, 0, 3 );
    QSpacerItem* spacer_4 = new QSpacerItem( 80, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout22->addItem( spacer_4, 2, 3 );

    resSlider = new QwtSlider( centralWidget(), "resSlider" );
    resSlider->setThumbLength( 16 );
    resSlider->setThumbWidth( 34 );
    resSlider->setBgStyle( QwtSlider::BgSlot );
    resSlider->setScalePos( QwtSlider::Bottom );

    Layout22->addMultiCellWidget( resSlider, 1, 1, 0, 3 );

    l5 = new QHBoxLayout( 0, 0, 6, "l5"); 

    gammaLabel = new QLabel( centralWidget(), "gammaLabel" );
    gammaLabel->setText( trUtf8( "c" ) );
    gammaLabel->setTextFormat( QLabel::RichText );
    gammaLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    l5->addWidget( gammaLabel );

    LCDNumber2_2 = new QLCDNumber( centralWidget(), "LCDNumber2_2" );
    LCDNumber2_2->setFrameShape( QLCDNumber::Panel );
    LCDNumber2_2->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2_2->setSmallDecimalPoint( TRUE );
    LCDNumber2_2->setSegmentStyle( QLCDNumber::Flat );
    l5->addWidget( LCDNumber2_2 );

    Layout22->addLayout( l5, 2, 2 );

    Layout5_4 = new QHBoxLayout( 0, 0, 6, "Layout5_4"); 

    alphaLabel = new QLabel( centralWidget(), "alphaLabel" );
    alphaLabel->setText( trUtf8( "a" ) );
    alphaLabel->setTextFormat( QLabel::RichText );
    alphaLabel->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    Layout5_4->addWidget( alphaLabel );

    LCDNumber2_4 = new QLCDNumber( centralWidget(), "LCDNumber2_4" );
    LCDNumber2_4->setFrameShape( QLCDNumber::Panel );
    LCDNumber2_4->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2_4->setSmallDecimalPoint( TRUE );
    LCDNumber2_4->setSegmentStyle( QLCDNumber::Flat );
    Layout5_4->addWidget( LCDNumber2_4 );

    Layout22->addLayout( Layout5_4, 2, 0 );

    Mesh2MainWindowBaseLayout->addLayout( Layout22, 1, 3 );
    QSpacerItem* spacer_5 = new QSpacerItem( 53, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Mesh2MainWindowBaseLayout->addItem( spacer_5, 1, 2 );

    Layout24 = new QGridLayout( 0, 1, 1, 0, 6, "Layout24"); 

    Layout5_6 = new QHBoxLayout( 0, 0, 6, "Layout5_6"); 

    TextLabel1_3_6 = new QLabel( centralWidget(), "TextLabel1_3_6" );
    TextLabel1_3_6->setText( trUtf8( "y:" ) );
    TextLabel1_3_6->setTextFormat( QLabel::AutoText );
    TextLabel1_3_6->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    Layout5_6->addWidget( TextLabel1_3_6 );

    LCDNumber2_6 = new QLCDNumber( centralWidget(), "LCDNumber2_6" );
    LCDNumber2_6->setFrameShape( QLCDNumber::Panel );
    LCDNumber2_6->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2_6->setSmallDecimalPoint( TRUE );
    LCDNumber2_6->setSegmentStyle( QLCDNumber::Flat );
    Layout5_6->addWidget( LCDNumber2_6 );

    Layout24->addMultiCellLayout( Layout5_6, 1, 1, 1, 2 );

    TextLabel2_3 = new QLabel( centralWidget(), "TextLabel2_3" );
    TextLabel2_3->setText( trUtf8( "X" ) );

    Layout24->addWidget( TextLabel2_3, 0, 2 );

    Layout16 = new QVBoxLayout( 0, 0, 6, "Layout16"); 

    zoomWheel = new QwtWheel( centralWidget(), "zoomWheel" );
    zoomWheel->setMinimumSize( QSize( 40, 10 ) );
    zoomWheel->setMaximumSize( QSize( 32767, 100 ) );
    zoomWheel->setCursor( QCursor( 0 ) );
    zoomWheel->setFocusPolicy( QwtWheel::StrongFocus );
    zoomWheel->setOrientation( QwtWheel::Vertical );
    zoomWheel->setTotalAngle( 720 );
    zoomWheel->setTickCnt( 8 );
    zoomWheel->setInternalBorder( 2 );
    Layout16->addWidget( zoomWheel );

    LCDNumber1 = new QLCDNumber( centralWidget(), "LCDNumber1" );
    LCDNumber1->setFrameShape( QLCDNumber::Panel );
    LCDNumber1->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber1->setSmallDecimalPoint( TRUE );
    LCDNumber1->setNumDigits( 3 );
    LCDNumber1->setSegmentStyle( QLCDNumber::Flat );
    LCDNumber1->setProperty( "intValue", 0 );
    Layout16->addWidget( LCDNumber1 );

    Layout24->addLayout( Layout16, 0, 4 );
    QSpacerItem* spacer_6 = new QSpacerItem( 70, 16, QSizePolicy::Fixed, QSizePolicy::Minimum );
    Layout24->addItem( spacer_6, 0, 3 );

    Layout17_2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout17_2"); 

    TextLabel1_3_5 = new QLabel( centralWidget(), "TextLabel1_3_5" );
    TextLabel1_3_5->setText( trUtf8( "z:" ) );
    TextLabel1_3_5->setTextFormat( QLabel::AutoText );
    TextLabel1_3_5->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );

    Layout17_2->addWidget( TextLabel1_3_5, 0, 0 );

    LCDNumber2_5 = new QLCDNumber( centralWidget(), "LCDNumber2_5" );
    LCDNumber2_5->setFrameShape( QLCDNumber::Panel );
    LCDNumber2_5->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2_5->setSmallDecimalPoint( TRUE );
    LCDNumber2_5->setSegmentStyle( QLCDNumber::Flat );

    Layout17_2->addWidget( LCDNumber2_5, 0, 1 );

    Layout24->addLayout( Layout17_2, 1, 3 );

    Layout12 = new QGridLayout( 0, 1, 1, 0, 6, "Layout12"); 

    TextLabel2_2 = new QLabel( centralWidget(), "TextLabel2_2" );
    TextLabel2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_2->setMinimumSize( QSize( 31, 25 ) );
    TextLabel2_2->setMaximumSize( QSize( 31, 25 ) );
    TextLabel2_2->setText( trUtf8( "rot" ) );
    TextLabel2_2->setAlignment( int( QLabel::AlignCenter ) );

    Layout12->addWidget( TextLabel2_2, 0, 1 );

    xR = new QwtWheel( centralWidget(), "xR" );
    xR->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, xR->sizePolicy().hasHeightForWidth() ) );
    xR->setMinimumSize( QSize( 100, 0 ) );
    xR->setMaximumSize( QSize( 100, 32767 ) );
    xR->setCursor( QCursor( 0 ) );
    xR->setFocusPolicy( QwtWheel::StrongFocus );
    xR->setMass( 0.1 );
    xR->setTickCnt( 10 );

    Layout12->addWidget( xR, 0, 0 );

    xS = new QwtWheel( centralWidget(), "xS" );
    xS->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, xS->sizePolicy().hasHeightForWidth() ) );
    xS->setMinimumSize( QSize( 100, 0 ) );
    xS->setMaximumSize( QSize( 100, 32767 ) );
    xS->setCursor( QCursor( 0 ) );
    xS->setFocusPolicy( QwtWheel::StrongFocus );
    xS->setMass( 0.1 );
    xS->setTickCnt( 20 );

    Layout12->addWidget( xS, 1, 0 );

    TextLabel2_2_2 = new QLabel( centralWidget(), "TextLabel2_2_2" );
    TextLabel2_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2_2_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_2_2->setMinimumSize( QSize( 31, 25 ) );
    TextLabel2_2_2->setMaximumSize( QSize( 31, 25 ) );
    TextLabel2_2_2->setText( trUtf8( "scale" ) );
    TextLabel2_2_2->setAlignment( int( QLabel::AlignCenter ) );

    Layout12->addWidget( TextLabel2_2_2, 2, 1 );

    TextLabel2_2_3 = new QLabel( centralWidget(), "TextLabel2_2_3" );
    TextLabel2_2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2_2_3->sizePolicy().hasHeightForWidth() ) );
    TextLabel2_2_3->setMinimumSize( QSize( 31, 25 ) );
    TextLabel2_2_3->setMaximumSize( QSize( 31, 25 ) );
    TextLabel2_2_3->setText( trUtf8( "shift" ) );
    TextLabel2_2_3->setAlignment( int( QLabel::AlignCenter ) );

    Layout12->addWidget( TextLabel2_2_3, 1, 1 );

    xSc = new QwtWheel( centralWidget(), "xSc" );
    xSc->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, xSc->sizePolicy().hasHeightForWidth() ) );
    xSc->setMinimumSize( QSize( 100, 0 ) );
    xSc->setMaximumSize( QSize( 100, 32767 ) );
    xSc->setCursor( QCursor( 0 ) );
    xSc->setFocusPolicy( QwtWheel::StrongFocus );
    xSc->setMass( 0.1 );
    xSc->setTickCnt( 20 );

    Layout12->addWidget( xSc, 2, 0 );

    Layout24->addMultiCellLayout( Layout12, 0, 0, 0, 1 );

    Layout11_2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout11_2"); 

    LCDNumber2 = new QLCDNumber( centralWidget(), "LCDNumber2" );
    LCDNumber2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, LCDNumber2->sizePolicy().hasHeightForWidth() ) );
    LCDNumber2->setFrameShape( QLCDNumber::Panel );
    LCDNumber2->setFrameShadow( QLCDNumber::Sunken );
    LCDNumber2->setSmallDecimalPoint( TRUE );
    LCDNumber2->setSegmentStyle( QLCDNumber::Flat );

    Layout11_2->addWidget( LCDNumber2, 0, 1 );

    TextLabel1_3 = new QLabel( centralWidget(), "TextLabel1_3" );
    TextLabel1_3->setText( trUtf8( "x:" ) );
    TextLabel1_3->setTextFormat( QLabel::AutoText );
    TextLabel1_3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );

    Layout11_2->addWidget( TextLabel1_3, 0, 0 );

    Layout24->addLayout( Layout11_2, 1, 0 );

    Mesh2MainWindowBaseLayout->addLayout( Layout24, 1, 1 );

    // actions
    openFile = new QAction( this, "openFile" );
    openFile->setIconSet( QIconSet( image1 ) );
    openFile->setText( trUtf8( "Open File" ) );
    openFile->setMenuText( trUtf8( "&Open File" ) );
    openFile->setAccel( 4194383 );
    Exit = new QAction( this, "Exit" );
    Exit->setText( trUtf8( "Exit" ) );
    Exit->setAccel( 4194385 );
    coord = new QActionGroup( this, "coord" );
    coord->setText( trUtf8( "Coordinates" ) );
    coord->setMenuText( trUtf8( "&Coord" ) );
    coord->setUsesDropDown( FALSE );
    Box = new QAction( coord, "Box" );
    Box->setToggleAction( TRUE );
    Box->setIconSet( QIconSet( image2 ) );
    Box->setText( trUtf8( "Box" ) );
    Box->setMenuText( trUtf8( "Box" ) );
    Box->setToolTip( trUtf8( "Box" ) );
    Box->setAccel( 65 );
    Frame = new QAction( coord, "Frame" );
    Frame->setToggleAction( TRUE );
    Frame->setIconSet( QIconSet( image3 ) );
    Frame->setText( trUtf8( "Frame" ) );
    Frame->setMenuText( trUtf8( "&Frame" ) );
    Frame->setToolTip( trUtf8( "Frame" ) );
    Frame->setAccel( 70 );
    None = new QAction( coord, "None" );
    None->setToggleAction( TRUE );
    None->setOn( TRUE );
    None->setIconSet( QIconSet( image4 ) );
    None->setText( trUtf8( "No Axes" ) );
    None->setToolTip( trUtf8( "No Axes" ) );
    animation = new QAction( this, "animation" );
    animation->setToggleAction( TRUE );
    animation->setIconSet( QIconSet( image5 ) );
    animation->setText( trUtf8( "Animation" ) );
    animation->setAccel( 4194369 );
    dump = new QAction( this, "dump" );
    dump->setIconSet( QIconSet( image6 ) );
    dump->setText( trUtf8( "Dump Pixmap" ) );
    plotstyle = new QActionGroup( this, "plotstyle" );
    plotstyle->setText( trUtf8( "Plot Style" ) );
    plotstyle->setUsesDropDown( FALSE );
    wireframe = new QAction( plotstyle, "wireframe" );
    wireframe->setToggleAction( TRUE );
    wireframe->setEnabled( TRUE );
    wireframe->setIconSet( QIconSet( image7 ) );
    wireframe->setText( trUtf8( "Wireframe" ) );
    wireframe->setToolTip( trUtf8( "Wireframe" ) );
    hiddenline = new QAction( plotstyle, "hiddenline" );
    hiddenline->setToggleAction( TRUE );
    hiddenline->setEnabled( TRUE );
    hiddenline->setIconSet( QIconSet( image8 ) );
    hiddenline->setText( trUtf8( "Hidden Line" ) );
    hiddenline->setToolTip( trUtf8( "Hidden Line (EXPERIMENTAL!)" ) );
    polygon = new QAction( plotstyle, "polygon" );
    polygon->setToggleAction( TRUE );
    polygon->setEnabled( TRUE );
    polygon->setIconSet( QIconSet( image9 ) );
    polygon->setText( trUtf8( "Polygon only" ) );
    polygon->setToolTip( trUtf8( "Polygon only" ) );
    filledmesh = new QAction( plotstyle, "filledmesh" );
    filledmesh->setToggleAction( TRUE );
    filledmesh->setOn( TRUE );
    filledmesh->setIconSet( QIconSet( image10 ) );
    filledmesh->setText( trUtf8( "Mesh & filled Polygons" ) );
    filledmesh->setToolTip( trUtf8( "Mesh & filled Polygons" ) );
    nodata = new QAction( plotstyle, "nodata" );
    nodata->setToggleAction( TRUE );
    nodata->setOn( FALSE );
    nodata->setIconSet( QIconSet( image11 ) );
    nodata->setText( trUtf8( "No Data" ) );
    nodata->setToolTip( trUtf8( "No Data" ) );
    color = new QActionGroup( this, "color" );
    color->setText( trUtf8( "Color" ) );
    color->setMenuText( trUtf8( "&Color" ) );
    color->setToolTip( trUtf8( "Color" ) );
    color->setExclusive( FALSE );
    color->setUsesDropDown( FALSE );
    axescolor = new QAction( color, "axescolor" );
    axescolor->setText( trUtf8( "Axes" ) );
    axescolor->setMenuText( trUtf8( "&Axes" ) );
    axescolor->setToolTip( trUtf8( "Axes" ) );
    backgroundcolor = new QAction( color, "backgroundcolor" );
    backgroundcolor->setText( trUtf8( "Background" ) );
    backgroundcolor->setMenuText( trUtf8( "&Background" ) );
    backgroundcolor->setToolTip( trUtf8( "Background" ) );
    backgroundcolor->setStatusTip( trUtf8( "Background" ) );
    meshcolor = new QAction( color, "meshcolor" );
    meshcolor->setText( trUtf8( "Mesh" ) );
    meshcolor->setMenuText( trUtf8( "&Mesh" ) );
    meshcolor->setToolTip( trUtf8( "Mesh" ) );
    numbercolor = new QAction( color, "numbercolor" );
    numbercolor->setText( trUtf8( "Numbers" ) );
    numbercolor->setMenuText( trUtf8( "&Numbers" ) );
    numbercolor->setToolTip( trUtf8( "Numbers" ) );
    labelcolor = new QAction( color, "labelcolor" );
    labelcolor->setText( trUtf8( "Label" ) );
    labelcolor->setMenuText( trUtf8( "&Label" ) );
    labelcolor->setToolTip( trUtf8( "Label" ) );
    resetcolor = new QAction( color, "resetcolor" );
    resetcolor->setText( trUtf8( "Reset" ) );
    resetcolor->setToolTip( trUtf8( "Reset" ) );
    alpha = new QAction( color, "alpha" );
    alpha->setText( trUtf8( "Alpha Values" ) );
    alpha->setMenuText( trUtf8( "Alpha Values ..." ) );
    alpha->setToolTip( trUtf8( "Alpha Values" ) );
    font = new QActionGroup( this, "font" );
    font->setText( trUtf8( "Fonts" ) );
    font->setMenuText( trUtf8( "&Fonts" ) );
    font->setExclusive( FALSE );
    font->setUsesDropDown( FALSE );
    numberfont = new QAction( font, "numberfont" );
    numberfont->setText( trUtf8( "Scale numbering" ) );
    numberfont->setMenuText( trUtf8( "&Scale numbering" ) );
    numberfont->setToolTip( trUtf8( "Scale numbering" ) );
    labelfont = new QAction( font, "labelfont" );
    labelfont->setText( trUtf8( "Axis label" ) );
    labelfont->setMenuText( trUtf8( "&Axis label" ) );
    labelfont->setToolTip( trUtf8( "Axis label" ) );
    resetfont = new QAction( font, "resetfont" );
    resetfont->setText( trUtf8( "Reset" ) );
    resetfont->setMenuText( trUtf8( "&Reset" ) );
    resetfont->setToolTip( trUtf8( "Reset" ) );
    floorstyle = new QActionGroup( this, "floorstyle" );
    floorstyle->setText( trUtf8( "Floor Style" ) );
    floorstyle->setUsesDropDown( FALSE );
    floordata = new QAction( floorstyle, "floordata" );
    floordata->setToggleAction( TRUE );
    floordata->setIconSet( QIconSet( image12 ) );
    floordata->setText( trUtf8( "Floor Data Projection" ) );
    floordata->setToolTip( trUtf8( "Floor Data Projection" ) );
    flooriso = new QAction( floorstyle, "flooriso" );
    flooriso->setToggleAction( TRUE );
    flooriso->setIconSet( QIconSet( image13 ) );
    flooriso->setText( trUtf8( "Floor Isolines" ) );
    flooriso->setToolTip( trUtf8( "Floor Isolines" ) );
    floormesh = new QAction( floorstyle, "floormesh" );
    floormesh->setToggleAction( TRUE );
    floormesh->setEnabled( FALSE );
    floormesh->setIconSet( QIconSet( image14 ) );
    floormesh->setText( trUtf8( "Floor Mesh" ) );
    floormesh->setToolTip( trUtf8( "Floor Mesh" ) );
    floormesh->setStatusTip( trUtf8( "Floor Mesh" ) );
    floornone = new QAction( floorstyle, "floornone" );
    floornone->setToggleAction( TRUE );
    floornone->setOn( TRUE );
    floornone->setIconSet( QIconSet( image15 ) );
    floornone->setText( trUtf8( "Empty Floor" ) );
    floornone->setToolTip( trUtf8( "Empty Floor" ) );


    // toolbars
    Toolbar = new QToolBar( "", this, DockTop ); 

    Toolbar->setCursor( QCursor( 0 ) );
    Toolbar->setLabel( trUtf8( "Toolbar" ) );
    openFile->addTo( Toolbar );

    functionCB = new QComboBox( FALSE, Toolbar, "functionCB" );
    functionCB->setMinimumSize( QSize( 150, 0 ) );
    functionCB->insertItem( trUtf8( "---" ) );
    functionCB->insertItem( trUtf8( "Hat" ) );
    functionCB->insertItem( trUtf8( "Rosenbrock" ) );
    animation->addTo( Toolbar );
    Toolbar->addSeparator();
    Box->addTo( Toolbar );
    Frame->addTo( Toolbar );
    None->addTo( Toolbar );
    Toolbar->addSeparator();
    wireframe->addTo( Toolbar );
    hiddenline->addTo( Toolbar );
    polygon->addTo( Toolbar );
    filledmesh->addTo( Toolbar );
    nodata->addTo( Toolbar );
    Toolbar->addSeparator();
    floordata->addTo( Toolbar );
    flooriso->addTo( Toolbar );
    floormesh->addTo( Toolbar );
    floornone->addTo( Toolbar );
    Toolbar->addSeparator();
    dump->addTo( Toolbar );


    // menubar
    menubar = new QMenuBar( this, "menubar" );

    file = new QPopupMenu( this ); 
    openFile->addTo( file );
    animation->addTo( file );
    Exit->addTo( file );
    menubar->insertItem( trUtf8( "&File" ), file );

    PopupMenu_3 = new QPopupMenu( this ); 
    axescolor->addTo( PopupMenu_3 );
    backgroundcolor->addTo( PopupMenu_3 );
    meshcolor->addTo( PopupMenu_3 );
    numbercolor->addTo( PopupMenu_3 );
    labelcolor->addTo( PopupMenu_3 );
    PopupMenu_3->insertSeparator();
    resetcolor->addTo( PopupMenu_3 );
    PopupMenu_3->insertSeparator();
    alpha->addTo( PopupMenu_3 );
    menubar->insertItem( trUtf8( "&Color" ), PopupMenu_3 );

    PopupMenu = new QPopupMenu( this ); 
    numberfont->addTo( PopupMenu );
    labelfont->addTo( PopupMenu );
    resetfont->addTo( PopupMenu );
    menubar->insertItem( trUtf8( "&Font" ), PopupMenu );



    // signals and slots connections
    connect( Exit, SIGNAL( activated() ), this, SLOT( close() ) );
    connect( zoomWheel, SIGNAL( valueChanged(double) ), LCDNumber1, SLOT( display(double) ) );
    connect( yS, SIGNAL( valueChanged(double) ), LCDNumber2_6, SLOT( display(double) ) );
    connect( xS, SIGNAL( valueChanged(double) ), LCDNumber2, SLOT( display(double) ) );
    connect( alpha, SIGNAL( activated() ), this, SLOT( showAlphaDlg() ) );
    connect( xR, SIGNAL( valueChanged(double) ), LCDNumber2_4, SLOT( display(double) ) );
    connect( yR, SIGNAL( valueChanged(double) ), LCDNumber2_3, SLOT( display(double) ) );
    connect( zR, SIGNAL( valueChanged(double) ), LCDNumber2_2, SLOT( display(double) ) );
    init();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Mesh2MainWindowBase::~Mesh2MainWindowBase()
{
    // no need to delete child widgets, Qt does it all for us
}

