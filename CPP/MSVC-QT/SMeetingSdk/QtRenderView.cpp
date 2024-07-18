﻿#include "QtRenderView.h"
#include <QDebug>

//看起来效果也是不错的呢，
//渲染一个画面只花费3%的cpu
QtRenderView::QtRenderView(QWidget *parent)
    : QWidget(parent)
{
    _w = _h = 0;
    _need_flash = false;
    connect(this,SIGNAL(onNeedUpdate()),this,SLOT(update()));
}

QtRenderView::~QtRenderView()
{
}
enum
{
    FLAGS = 0x40080100
};
#define READUV(U,V) (tables[256 + (U)] + tables[512 + (V)])
#define READY(Y)    tables[Y]
#define FIXUP(Y)                 \
do {                             \
        int tmp = (Y) & FLAGS;       \
        if (tmp != 0)                \
    {                            \
            tmp  -= tmp>>8;          \
            (Y)  |= tmp;             \
            tmp   = FLAGS & ~(Y>>1); \
            (Y)  += tmp>>8;          \
    }                            \
} while (0 == 1)

#define STORE(Y,DSTPTR)     \
    do {                        \
        (DSTPTR) = (Y & 0xFF) | (0xFF00 & (Y>>14)) | (0xFF0000 & (Y<<5));\
} while (0 == 1)


    uint32_t yuv2rgb565_table[256 * 3] =
        {
            /* y_table */
            0x7FFFFFEDU,
            0x7FFFFFEFU,
            0x7FFFFFF0U,
            0x7FFFFFF1U,
            0x7FFFFFF2U,
            0x7FFFFFF3U,
            0x7FFFFFF4U,
            0x7FFFFFF6U,
            0x7FFFFFF7U,
            0x7FFFFFF8U,
            0x7FFFFFF9U,
            0x7FFFFFFAU,
            0x7FFFFFFBU,
            0x7FFFFFFDU,
            0x7FFFFFFEU,
            0x7FFFFFFFU,
            0x80000000U,
            0x80400801U,
            0x80A01002U,
            0x80E01803U,
            0x81202805U,
            0x81803006U,
            0x81C03807U,
            0x82004008U,
            0x82604809U,
            0x82A0500AU,
            0x82E0600CU,
            0x8340680DU,
            0x8380700EU,
            0x83C0780FU,
            0x84208010U,
            0x84608811U,
            0x84A09813U,
            0x8500A014U,
            0x8540A815U,
            0x8580B016U,
            0x85E0B817U,
            0x8620C018U,
            0x8660D01AU,
            0x86C0D81BU,
            0x8700E01CU,
            0x8740E81DU,
            0x87A0F01EU,
            0x87E0F81FU,
            0x88210821U,
            0x88811022U,
            0x88C11823U,
            0x89012024U,
            0x89412825U,
            0x89A13026U,
            0x89E14028U,
            0x8A214829U,
            0x8A81502AU,
            0x8AC1582BU,
            0x8B01602CU,
            0x8B61682DU,
            0x8BA1782FU,
            0x8BE18030U,
            0x8C418831U,
            0x8C819032U,
            0x8CC19833U,
            0x8D21A034U,
            0x8D61B036U,
            0x8DA1B837U,
            0x8E01C038U,
            0x8E41C839U,
            0x8E81D03AU,
            0x8EE1D83BU,
            0x8F21E83DU,
            0x8F61F03EU,
            0x8FC1F83FU,
            0x90020040U,
            0x90420841U,
            0x90A21042U,
            0x90E22044U,
            0x91222845U,
            0x91823046U,
            0x91C23847U,
            0x92024048U,
            0x92624849U,
            0x92A2504AU,
            0x92E2604CU,
            0x9342684DU,
            0x9382704EU,
            0x93C2784FU,
            0x94228050U,
            0x94628851U,
            0x94A29853U,
            0x9502A054U,
            0x9542A855U,
            0x9582B056U,
            0x95E2B857U,
            0x9622C058U,
            0x9662D05AU,
            0x96C2D85BU,
            0x9702E05CU,
            0x9742E85DU,
            0x97A2F05EU,
            0x97E2F85FU,
            0x98230861U,
            0x98831062U,
            0x98C31863U,
            0x99032064U,
            0x99632865U,
            0x99A33066U,
            0x99E34068U,
            0x9A434869U,
            0x9A83506AU,
            0x9AC3586BU,
            0x9B23606CU,
            0x9B63686DU,
            0x9BA3786FU,
            0x9BE38070U,
            0x9C438871U,
            0x9C839072U,
            0x9CC39873U,
            0x9D23A074U,
            0x9D63B076U,
            0x9DA3B877U,
            0x9E03C078U,
            0x9E43C879U,
            0x9E83D07AU,
            0x9EE3D87BU,
            0x9F23E87DU,
            0x9F63F07EU,
            0x9FC3F87FU,
            0xA0040080U,
            0xA0440881U,
            0xA0A41082U,
            0xA0E42084U,
            0xA1242885U,
            0xA1843086U,
            0xA1C43887U,
            0xA2044088U,
            0xA2644889U,
            0xA2A4588BU,
            0xA2E4608CU,
            0xA344688DU,
            0xA384708EU,
            0xA3C4788FU,
            0xA4248090U,
            0xA4649092U,
            0xA4A49893U,
            0xA504A094U,
            0xA544A895U,
            0xA584B096U,
            0xA5E4B897U,
            0xA624C098U,
            0xA664D09AU,
            0xA6C4D89BU,
            0xA704E09CU,
            0xA744E89DU,
            0xA7A4F09EU,
            0xA7E4F89FU,
            0xA82508A1U,
            0xA88510A2U,
            0xA8C518A3U,
            0xA90520A4U,
            0xA96528A5U,
            0xA9A530A6U,
            0xA9E540A8U,
            0xAA4548A9U,
            0xAA8550AAU,
            0xAAC558ABU,
            0xAB2560ACU,
            0xAB6568ADU,
            0xABA578AFU,
            0xAC0580B0U,
            0xAC4588B1U,
            0xAC8590B2U,
            0xACE598B3U,
            0xAD25A0B4U,
            0xAD65B0B6U,
            0xADA5B8B7U,
            0xAE05C0B8U,
            0xAE45C8B9U,
            0xAE85D0BAU,
            0xAEE5D8BBU,
            0xAF25E8BDU,
            0xAF65F0BEU,
            0xAFC5F8BFU,
            0xB00600C0U,
            0xB04608C1U,
            0xB0A610C2U,
            0xB0E620C4U,
            0xB12628C5U,
            0xB18630C6U,
            0xB1C638C7U,
            0xB20640C8U,
            0xB26648C9U,
            0xB2A658CBU,
            0xB2E660CCU,
            0xB34668CDU,
            0xB38670CEU,
            0xB3C678CFU,
            0xB42680D0U,
            0xB46690D2U,
            0xB4A698D3U,
            0xB506A0D4U,
            0xB546A8D5U,
            0xB586B0D6U,
            0xB5E6B8D7U,
            0xB626C8D9U,
            0xB666D0DAU,
            0xB6C6D8DBU,
            0xB706E0DCU,
            0xB746E8DDU,
            0xB7A6F0DEU,
            0xB7E6F8DFU,
            0xB82708E1U,
            0xB88710E2U,
            0xB8C718E3U,
            0xB90720E4U,
            0xB96728E5U,
            0xB9A730E6U,
            0xB9E740E8U,
            0xBA4748E9U,
            0xBA8750EAU,
            0xBAC758EBU,
            0xBB2760ECU,
            0xBB6768EDU,
            0xBBA778EFU,
            0xBC0780F0U,
            0xBC4788F1U,
            0xBC8790F2U,
            0xBCE798F3U,
            0xBD27A0F4U,
            0xBD67B0F6U,
            0xBDC7B8F7U,
            0xBE07C0F8U,
            0xBE47C8F9U,
            0xBEA7D0FAU,
            0xBEE7D8FBU,
            0xBF27E8FDU,
            0xBF87F0FEU,
            0xBFC7F8FFU,
            0xC0080100U,
            0xC0480901U,
            0xC0A81102U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            0xC0E82104U,
            /* u_table */
            0x0C400103U,
            0x0C200105U,
            0x0C200107U,
            0x0C000109U,
            0x0BE0010BU,
            0x0BC0010DU,
            0x0BA0010FU,
            0x0BA00111U,
            0x0B800113U,
            0x0B600115U,
            0x0B400117U,
            0x0B400119U,
            0x0B20011BU,
            0x0B00011DU,
            0x0AE0011FU,
            0x0AE00121U,
            0x0AC00123U,
            0x0AA00125U,
            0x0A800127U,
            0x0A600129U,
            0x0A60012BU,
            0x0A40012DU,
            0x0A20012FU,
            0x0A000131U,
            0x0A000132U,
            0x09E00134U,
            0x09C00136U,
            0x09A00138U,
            0x09A0013AU,
            0x0980013CU,
            0x0960013EU,
            0x09400140U,
            0x09400142U,
            0x09200144U,
            0x09000146U,
            0x08E00148U,
            0x08C0014AU,
            0x08C0014CU,
            0x08A0014EU,
            0x08800150U,
            0x08600152U,
            0x08600154U,
            0x08400156U,
            0x08200158U,
            0x0800015AU,
            0x0800015CU,
            0x07E0015EU,
            0x07C00160U,
            0x07A00162U,
            0x07A00164U,
            0x07800166U,
            0x07600168U,
            0x0740016AU,
            0x0720016CU,
            0x0720016EU,
            0x07000170U,
            0x06E00172U,
            0x06C00174U,
            0x06C00176U,
            0x06A00178U,
            0x0680017AU,
            0x0660017CU,
            0x0660017EU,
            0x06400180U,
            0x06200182U,
            0x06000184U,
            0x05E00185U,
            0x05E00187U,
            0x05C00189U,
            0x05A0018BU,
            0x0580018DU,
            0x0580018FU,
            0x05600191U,
            0x05400193U,
            0x05200195U,
            0x05200197U,
            0x05000199U,
            0x04E0019BU,
            0x04C0019DU,
            0x04C0019FU,
            0x04A001A1U,
            0x048001A3U,
            0x046001A5U,
            0x044001A7U,
            0x044001A9U,
            0x042001ABU,
            0x040001ADU,
            0x03E001AFU,
            0x03E001B1U,
            0x03C001B3U,
            0x03A001B5U,
            0x038001B7U,
            0x038001B9U,
            0x036001BBU,
            0x034001BDU,
            0x032001BFU,
            0x032001C1U,
            0x030001C3U,
            0x02E001C5U,
            0x02C001C7U,
            0x02A001C9U,
            0x02A001CBU,
            0x028001CDU,
            0x026001CFU,
            0x024001D1U,
            0x024001D3U,
            0x022001D5U,
            0x020001D7U,
            0x01E001D8U,
            0x01E001DAU,
            0x01C001DCU,
            0x01A001DEU,
            0x018001E0U,
            0x016001E2U,
            0x016001E4U,
            0x014001E6U,
            0x012001E8U,
            0x010001EAU,
            0x010001ECU,
            0x00E001EEU,
            0x00C001F0U,
            0x00A001F2U,
            0x00A001F4U,
            0x008001F6U,
            0x006001F8U,
            0x004001FAU,
            0x004001FCU,
            0x002001FEU,
            0x00000200U,
            0xFFE00202U,
            0xFFC00204U,
            0xFFC00206U,
            0xFFA00208U,
            0xFF80020AU,
            0xFF60020CU,
            0xFF60020EU,
            0xFF400210U,
            0xFF200212U,
            0xFF000214U,
            0xFF000216U,
            0xFEE00218U,
            0xFEC0021AU,
            0xFEA0021CU,
            0xFEA0021EU,
            0xFE800220U,
            0xFE600222U,
            0xFE400224U,
            0xFE200226U,
            0xFE200228U,
            0xFE000229U,
            0xFDE0022BU,
            0xFDC0022DU,
            0xFDC0022FU,
            0xFDA00231U,
            0xFD800233U,
            0xFD600235U,
            0xFD600237U,
            0xFD400239U,
            0xFD20023BU,
            0xFD00023DU,
            0xFCE0023FU,
            0xFCE00241U,
            0xFCC00243U,
            0xFCA00245U,
            0xFC800247U,
            0xFC800249U,
            0xFC60024BU,
            0xFC40024DU,
            0xFC20024FU,
            0xFC200251U,
            0xFC000253U,
            0xFBE00255U,
            0xFBC00257U,
            0xFBC00259U,
            0xFBA0025BU,
            0xFB80025DU,
            0xFB60025FU,
            0xFB400261U,
            0xFB400263U,
            0xFB200265U,
            0xFB000267U,
            0xFAE00269U,
            0xFAE0026BU,
            0xFAC0026DU,
            0xFAA0026FU,
            0xFA800271U,
            0xFA800273U,
            0xFA600275U,
            0xFA400277U,
            0xFA200279U,
            0xFA20027BU,
            0xFA00027CU,
            0xF9E0027EU,
            0xF9C00280U,
            0xF9A00282U,
            0xF9A00284U,
            0xF9800286U,
            0xF9600288U,
            0xF940028AU,
            0xF940028CU,
            0xF920028EU,
            0xF9000290U,
            0xF8E00292U,
            0xF8E00294U,
            0xF8C00296U,
            0xF8A00298U,
            0xF880029AU,
            0xF860029CU,
            0xF860029EU,
            0xF84002A0U,
            0xF82002A2U,
            0xF80002A4U,
            0xF80002A6U,
            0xF7E002A8U,
            0xF7C002AAU,
            0xF7A002ACU,
            0xF7A002AEU,
            0xF78002B0U,
            0xF76002B2U,
            0xF74002B4U,
            0xF74002B6U,
            0xF72002B8U,
            0xF70002BAU,
            0xF6E002BCU,
            0xF6C002BEU,
            0xF6C002C0U,
            0xF6A002C2U,
            0xF68002C4U,
            0xF66002C6U,
            0xF66002C8U,
            0xF64002CAU,
            0xF62002CCU,
            0xF60002CEU,
            0xF60002CFU,
            0xF5E002D1U,
            0xF5C002D3U,
            0xF5A002D5U,
            0xF5A002D7U,
            0xF58002D9U,
            0xF56002DBU,
            0xF54002DDU,
            0xF52002DFU,
            0xF52002E1U,
            0xF50002E3U,
            0xF4E002E5U,
            0xF4C002E7U,
            0xF4C002E9U,
            0xF4A002EBU,
            0xF48002EDU,
            0xF46002EFU,
            0xF46002F1U,
            0xF44002F3U,
            0xF42002F5U,
            0xF40002F7U,
            0xF3E002F9U,
            0xF3E002FBU,
            /* v_table */
            0x1A09A000U,
            0x19E9A800U,
            0x19A9B800U,
            0x1969C800U,
            0x1949D000U,
            0x1909E000U,
            0x18C9E800U,
            0x18A9F800U,
            0x186A0000U,
            0x182A1000U,
            0x180A2000U,
            0x17CA2800U,
            0x17AA3800U,
            0x176A4000U,
            0x172A5000U,
            0x170A6000U,
            0x16CA6800U,
            0x168A7800U,
            0x166A8000U,
            0x162A9000U,
            0x160AA000U,
            0x15CAA800U,
            0x158AB800U,
            0x156AC000U,
            0x152AD000U,
            0x14EAE000U,
            0x14CAE800U,
            0x148AF800U,
            0x146B0000U,
            0x142B1000U,
            0x13EB2000U,
            0x13CB2800U,
            0x138B3800U,
            0x134B4000U,
            0x132B5000U,
            0x12EB6000U,
            0x12CB6800U,
            0x128B7800U,
            0x124B8000U,
            0x122B9000U,
            0x11EBA000U,
            0x11ABA800U,
            0x118BB800U,
            0x114BC000U,
            0x112BD000U,
            0x10EBE000U,
            0x10ABE800U,
            0x108BF800U,
            0x104C0000U,
            0x100C1000U,
            0x0FEC2000U,
            0x0FAC2800U,
            0x0F8C3800U,
            0x0F4C4000U,
            0x0F0C5000U,
            0x0EEC5800U,
            0x0EAC6800U,
            0x0E6C7800U,
            0x0E4C8000U,
            0x0E0C9000U,
            0x0DEC9800U,
            0x0DACA800U,
            0x0D6CB800U,
            0x0D4CC000U,
            0x0D0CD000U,
            0x0CCCD800U,
            0x0CACE800U,
            0x0C6CF800U,
            0x0C4D0000U,
            0x0C0D1000U,
            0x0BCD1800U,
            0x0BAD2800U,
            0x0B6D3800U,
            0x0B2D4000U,
            0x0B0D5000U,
            0x0ACD5800U,
            0x0AAD6800U,
            0x0A6D7800U,
            0x0A2D8000U,
            0x0A0D9000U,
            0x09CD9800U,
            0x098DA800U,
            0x096DB800U,
            0x092DC000U,
            0x090DD000U,
            0x08CDD800U,
            0x088DE800U,
            0x086DF800U,
            0x082E0000U,
            0x07EE1000U,
            0x07CE1800U,
            0x078E2800U,
            0x076E3800U,
            0x072E4000U,
            0x06EE5000U,
            0x06CE5800U,
            0x068E6800U,
            0x064E7800U,
            0x062E8000U,
            0x05EE9000U,
            0x05CE9800U,
            0x058EA800U,
            0x054EB800U,
            0x052EC000U,
            0x04EED000U,
            0x04AED800U,
            0x048EE800U,
            0x044EF000U,
            0x042F0000U,
            0x03EF1000U,
            0x03AF1800U,
            0x038F2800U,
            0x034F3000U,
            0x030F4000U,
            0x02EF5000U,
            0x02AF5800U,
            0x028F6800U,
            0x024F7000U,
            0x020F8000U,
            0x01EF9000U,
            0x01AF9800U,
            0x016FA800U,
            0x014FB000U,
            0x010FC000U,
            0x00EFD000U,
            0x00AFD800U,
            0x006FE800U,
            0x004FF000U,
            0x00100000U,
            0xFFD01000U,
            0xFFB01800U,
            0xFF702800U,
            0xFF303000U,
            0xFF104000U,
            0xFED05000U,
            0xFEB05800U,
            0xFE706800U,
            0xFE307000U,
            0xFE108000U,
            0xFDD09000U,
            0xFD909800U,
            0xFD70A800U,
            0xFD30B000U,
            0xFD10C000U,
            0xFCD0D000U,
            0xFC90D800U,
            0xFC70E800U,
            0xFC30F000U,
            0xFBF10000U,
            0xFBD11000U,
            0xFB911800U,
            0xFB712800U,
            0xFB313000U,
            0xFAF14000U,
            0xFAD14800U,
            0xFA915800U,
            0xFA516800U,
            0xFA317000U,
            0xF9F18000U,
            0xF9D18800U,
            0xF9919800U,
            0xF951A800U,
            0xF931B000U,
            0xF8F1C000U,
            0xF8B1C800U,
            0xF891D800U,
            0xF851E800U,
            0xF831F000U,
            0xF7F20000U,
            0xF7B20800U,
            0xF7921800U,
            0xF7522800U,
            0xF7123000U,
            0xF6F24000U,
            0xF6B24800U,
            0xF6925800U,
            0xF6526800U,
            0xF6127000U,
            0xF5F28000U,
            0xF5B28800U,
            0xF5729800U,
            0xF552A800U,
            0xF512B000U,
            0xF4F2C000U,
            0xF4B2C800U,
            0xF472D800U,
            0xF452E800U,
            0xF412F000U,
            0xF3D30000U,
            0xF3B30800U,
            0xF3731800U,
            0xF3532800U,
            0xF3133000U,
            0xF2D34000U,
            0xF2B34800U,
            0xF2735800U,
            0xF2336800U,
            0xF2137000U,
            0xF1D38000U,
            0xF1B38800U,
            0xF1739800U,
            0xF133A800U,
            0xF113B000U,
            0xF0D3C000U,
            0xF093C800U,
            0xF073D800U,
            0xF033E000U,
            0xF013F000U,
            0xEFD40000U,
            0xEF940800U,
            0xEF741800U,
            0xEF342000U,
            0xEEF43000U,
            0xEED44000U,
            0xEE944800U,
            0xEE745800U,
            0xEE346000U,
            0xEDF47000U,
            0xEDD48000U,
            0xED948800U,
            0xED549800U,
            0xED34A000U,
            0xECF4B000U,
            0xECD4C000U,
            0xEC94C800U,
            0xEC54D800U,
            0xEC34E000U,
            0xEBF4F000U,
            0xEBB50000U,
            0xEB950800U,
            0xEB551800U,
            0xEB352000U,
            0xEAF53000U,
            0xEAB54000U,
            0xEA954800U,
            0xEA555800U,
            0xEA156000U,
            0xE9F57000U,
            0xE9B58000U,
            0xE9958800U,
            0xE9559800U,
            0xE915A000U,
            0xE8F5B000U,
            0xE8B5C000U,
            0xE875C800U,
            0xE855D800U,
            0xE815E000U,
            0xE7F5F000U,
            0xE7B60000U,
            0xE7760800U,
            0xE7561800U,
            0xE7162000U,
            0xE6D63000U,
            0xE6B64000U,
            0xE6764800U,
            0xE6365800U
    };

void yuv420_2_rgb8888(uint8_t* dst_ptr_,
                                     const uint8_t* y_ptr,
                                     const uint8_t* u_ptr,
                                     const uint8_t* v_ptr,
                                     int32_t   width,
                                     int32_t   height)
{
    uint32_t* tables = yuv2rgb565_table;
    uint32_t* dst_ptr = (uint32_t*)(void*)dst_ptr_;
    int32_t   y_span = width;
    int32_t   uv_span = width >> 1;
    int32_t   dst_span = width << 2;
    dst_span >>= 2;

    height -= 1;
    while (height > 0)
    {
        height -= width << 16;
        height += 1 << 16;
        while (height < 0)
        {
            /* Do 2 column pairs */
            uint32_t uv, y0, y1;

            uv = READUV(*u_ptr++, *v_ptr++);
            y1 = uv + READY(y_ptr[y_span]);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, dst_ptr[dst_span]);
            STORE(y0, *dst_ptr++);
            y1 = uv + READY(y_ptr[y_span]);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, dst_ptr[dst_span]);
            STORE(y0, *dst_ptr++);
            height += (2 << 16);
        }
        if ((height >> 16) == 0)
        {
            /* Trailing column pair */
            uint32_t uv, y0, y1;

            uv = READUV(*u_ptr, *v_ptr);
            y1 = uv + READY(y_ptr[y_span]);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y0, dst_ptr[dst_span]);
            STORE(y1, *dst_ptr++);
        }
        dst_ptr += dst_span * 2 - width;
        y_ptr += y_span * 2 - width;
        u_ptr += uv_span - (width >> 1);
        v_ptr += uv_span - (width >> 1);
        height = (height << 16) >> 16;
        height -= 2;
    }
    if (height == 0)
    {
        /* Trail row */
        height -= width << 16;
        height += 1 << 16;
        while (height < 0)
        {
            /* Do a row pair */
            uint32_t uv, y0, y1;

            uv = READUV(*u_ptr++, *v_ptr++);
            y1 = uv + READY(*y_ptr++);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, *dst_ptr++);
            STORE(y0, *dst_ptr++);
            height += (2 << 16);
        }
        if ((height >> 16) == 0)
        {
            /* Trailing pix */
            uint32_t uv, y0;

            uv = READUV(*u_ptr++, *v_ptr++);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y0);
            STORE(y0, *dst_ptr++);
        }
    }
}
void BlitYUY2(void* source, void* dest, int larg, int haut, int pitch)
{


    // 	_int64 R1B0 = 0x0198010002140100;
    //	_int64 G1G0 = 0x006400D0006400D0;
    _int64 R1B0 = 0x00CC0095010A0095;
    //	_int64 G1G0 = 0xFF980032FF980032;
    //	_int64 Y1Y0 = 0x0000010000000100;
    //	_int64 Y1Y0 = 0x0000009500000095;
    _int64 YU = 0xFFCE0095FFCE0095;
    _int64 GV0 = 0x0000FF980000FF98;
    _int64 GAND = 0x0000FF000000FF00;
    // 	_int64 RBAND = 0x00FF00FF00FF00FF;
    _int64 S128 = 0x0080001000800010;
    _asm
    {
        push	ebx
        push	ecx
        push	edx
        push	esi
        push	edi

        mov		esi, source; pointe sur source
        mov     edi, dest
        mov     edx, pitch
        sub     edx, larg; modulo
        pxor	mm7, mm7
        shl     edx, 2; en octet
        mov     ebx, haut
        bcl :
        mov     ecx, larg
            shr     ecx, 4
            bcl1 :
            prefetchnta[esi + 32]
            movq		mm0, [esi]; V0, Y1, U0, Y0
            movq		mm4, mm0
            punpcklbw	mm0, mm7; passe en 16bit

            punpckhbw	mm4, mm7
            psubsw		mm0, S128
            psubsw		mm4, S128

            pshufw		mm1, mm0, 064h; U0, Y1, U0, Y0
            pshufw		mm5, mm4, 064h
            pshufw		mm2, mm0, 0C6h; V0, Y0, U0, Y1

            pmaddwd		mm0, R1B0; R1, B0
            pshufw		mm3, mm2, 0FFh; V0, V0, V0, V0
            pmaddwd		mm1, YU; 0, Y1 + U0, 0, Y0 + U0

            pmaddwd		mm5, YU; 0, Y1 + U0, 0, Y0 + U0
            pshufw		mm6, mm4, 0C6h; V0, Y0, U0, Y1
            pmaddwd		mm4, R1B0; R1, B0

            psrad		mm0, 7
            pmaddwd		mm3, GV0
            paddd		mm1, mm3; G1, G0

            pshufw		mm3, mm6, 0FFh; V0, V0, V0, V0
            pmaddwd		mm2, R1B0; R0, B1
            psrad		mm4, 7

            psrad		mm2, 7
            psrad		mm1, 7
            pmaddwd		mm3, GV0

            pmaddwd		mm6, R1B0; R0, B1
            packuswb	mm1, mm1
            paddd		mm5, mm3; G1, G0

            packuswb	mm0, mm2; R0, B1, R1, B0
            psrad		mm6, 7
            psrad		mm5, 7

            punpcklbw	mm1, mm1
            pshufw		mm0, mm0, 06Ch; R1, B1, R0, B0
            packuswb	mm5, mm5

            pand		mm1, GAND
            add			esi, 8
            packuswb	mm4, mm6; R0, B1, R1, B0

            por			mm0, mm1
            punpcklbw	mm5, mm5
            pshufw		mm4, mm4, 06Ch; R1, B1, R0, B0

            movntq[edi], mm0
            pand		mm5, GAND
            por			mm4, mm5

            movntq[edi + 8], mm4
            add         edi, 16
            ; --------------------------------------------------------------------------------------
            movq		mm0, [esi]; V0, Y1, U0, Y0
            movq		mm4, mm0
            punpcklbw	mm0, mm7; passe en 16bit

            punpckhbw	mm4, mm7
            psubsw		mm0, S128
            psubsw		mm4, S128

            pshufw		mm1, mm0, 064h; U0, Y1, U0, Y0
            pshufw		mm5, mm4, 064h
            pshufw		mm2, mm0, 0C6h; V0, Y0, U0, Y1

            pmaddwd		mm0, R1B0; R1, B0
            pshufw		mm3, mm2, 0FFh; V0, V0, V0, V0
            pmaddwd		mm1, YU; 0, Y1 + U0, 0, Y0 + U0

            pmaddwd		mm5, YU; 0, Y1 + U0, 0, Y0 + U0
            pshufw		mm6, mm4, 0C6h; V0, Y0, U0, Y1
            pmaddwd		mm4, R1B0; R1, B0

            psrad		mm0, 7
            pmaddwd		mm3, GV0
            paddd		mm1, mm3; G1, G0

            pshufw		mm3, mm6, 0FFh; V0, V0, V0, V0
            pmaddwd		mm2, R1B0; R0, B1
            psrad		mm4, 7

            psrad		mm2, 7
            psrad		mm1, 7
            pmaddwd		mm3, GV0

            pmaddwd		mm6, R1B0; R0, B1
            packuswb	mm1, mm1
            paddd		mm5, mm3; G1, G0

            packuswb	mm0, mm2; R0, B1, R1, B0
            psrad		mm6, 7
            psrad		mm5, 7

            punpcklbw	mm1, mm1
            pshufw		mm0, mm0, 06Ch; R1, B1, R0, B0
            packuswb	mm5, mm5

            pand		mm1, GAND
            add			esi, 8
            packuswb	mm4, mm6; R0, B1, R1, B0

            por			mm0, mm1
            punpcklbw	mm5, mm5
            pshufw		mm4, mm4, 06Ch; R1, B1, R0, B0

            movntq[edi], mm0
            pand		mm5, GAND
            por			mm4, mm5

            movntq[edi + 8], mm4
            add         edi, 16

            ; --------------------------------------------------------------------------------------
            movq		mm0, [esi]; V0, Y1, U0, Y0
            movq		mm4, mm0
            punpcklbw	mm0, mm7; passe en 16bit

            punpckhbw	mm4, mm7
            psubsw		mm0, S128
            psubsw		mm4, S128

            pshufw		mm1, mm0, 064h; U0, Y1, U0, Y0
            pshufw		mm5, mm4, 064h
            pshufw		mm2, mm0, 0C6h; V0, Y0, U0, Y1

            pmaddwd		mm0, R1B0; R1, B0
            pshufw		mm3, mm2, 0FFh; V0, V0, V0, V0
            pmaddwd		mm1, YU; 0, Y1 + U0, 0, Y0 + U0

            pmaddwd		mm5, YU; 0, Y1 + U0, 0, Y0 + U0
            pshufw		mm6, mm4, 0C6h; V0, Y0, U0, Y1
            pmaddwd		mm4, R1B0; R1, B0

            psrad		mm0, 7
            pmaddwd		mm3, GV0
            paddd		mm1, mm3; G1, G0

            pshufw		mm3, mm6, 0FFh; V0, V0, V0, V0
            pmaddwd		mm2, R1B0; R0, B1
            psrad		mm4, 7

            psrad		mm2, 7
            psrad		mm1, 7
            pmaddwd		mm3, GV0

            pmaddwd		mm6, R1B0; R0, B1
            packuswb	mm1, mm1
            paddd		mm5, mm3; G1, G0

            packuswb	mm0, mm2; R0, B1, R1, B0
            psrad		mm6, 7
            psrad		mm5, 7

            punpcklbw	mm1, mm1
            pshufw		mm0, mm0, 06Ch; R1, B1, R0, B0
            packuswb	mm5, mm5

            pand		mm1, GAND
            add			esi, 8
            packuswb	mm4, mm6; R0, B1, R1, B0

            por			mm0, mm1
            punpcklbw	mm5, mm5
            pshufw		mm4, mm4, 06Ch; R1, B1, R0, B0

            movntq[edi], mm0
            pand		mm5, GAND
            por			mm4, mm5

            movntq[edi + 8], mm4
            add         edi, 16
            ; --------------------------------------------------------------------------------------
            movq		mm0, [esi]; V0, Y1, U0, Y0
            movq		mm4, mm0
            punpcklbw	mm0, mm7; passe en 16bit

            punpckhbw	mm4, mm7
            psubsw		mm0, S128
            psubsw		mm4, S128

            pshufw		mm1, mm0, 064h; U0, Y1, U0, Y0
            pshufw		mm5, mm4, 064h
            pshufw		mm2, mm0, 0C6h; V0, Y0, U0, Y1

            pmaddwd		mm0, R1B0; R1, B0
            pshufw		mm3, mm2, 0FFh; V0, V0, V0, V0
            pmaddwd		mm1, YU; 0, Y1 + U0, 0, Y0 + U0

            pmaddwd		mm5, YU; 0, Y1 + U0, 0, Y0 + U0
            pshufw		mm6, mm4, 0C6h; V0, Y0, U0, Y1
            pmaddwd		mm4, R1B0; R1, B0

            psrad		mm0, 7
            pmaddwd		mm3, GV0
            paddd		mm1, mm3; G1, G0

            pshufw		mm3, mm6, 0FFh; V0, V0, V0, V0
            pmaddwd		mm2, R1B0; R0, B1
            psrad		mm4, 7

            psrad		mm2, 7
            psrad		mm1, 7
            pmaddwd		mm3, GV0

            pmaddwd		mm6, R1B0; R0, B1
            packuswb	mm1, mm1
            paddd		mm5, mm3; G1, G0

            packuswb	mm0, mm2; R0, B1, R1, B0
            psrad		mm6, 7
            psrad		mm5, 7

            punpcklbw	mm1, mm1
            pshufw		mm0, mm0, 06Ch; R1, B1, R0, B0
            packuswb	mm5, mm5

            pand		mm1, GAND
            add			esi, 8
            packuswb	mm4, mm6; R0, B1, R1, B0

            por			mm0, mm1
            punpcklbw	mm5, mm5
            pshufw		mm4, mm4, 06Ch; R1, B1, R0, B0

            movntq[edi], mm0
            pand		mm5, GAND
            por			mm4, mm5

            movntq[edi + 8], mm4
            add         edi, 16


            dec		ecx
            jne     bcl1
            add     edi, edx
            dec     ebx
            jne     bcl

            emms
            pop		edi
            pop		esi
            pop		edx
            pop		ecx
            pop		ebx
    }
    return;
}

void QtRenderView::updateFull(int R, int G, int B)
{
    //if (_w == 0 || _h == 0) {
    //    return;
    //}
    //if (_buf_len < (_w * _h*3/2))
    //{
    //    if (_buf) {
    //        free(_buf);
    //    }
    //    _buf = (unsigned char*)malloc(_w*_h*3/2);
    //}
    //int Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
    //int U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
    //int V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
    //int size = _w * _h;
    //memset(_buf, Y, size);
    //memset(_buf + size, U,  size / 4);
    //memset(_buf + size * 5 / 4, V, size / 4);

    //updatePlanes(_buf,0,0,0,_w,_h,0,_label);
    Flash_Rgb[0] = R;
    Flash_Rgb[1] = G;
    Flash_Rgb[2] = B;
    _need_flash = true;
    _mutex.lock();
    if (_img) {
        delete  _img;
        _img = nullptr;
    }
    _mutex.unlock();
    _w = _h = 0;
    emit onNeedUpdate();
}

void NV21_T_RGB(const unsigned char* Y, unsigned char* rgb,unsigned int width, unsigned int height)
{
    const unsigned char* UV = Y + width*height;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int yIndex = y * width + x;
            int uvOffset = (y / 2) * width + (x / 2) * 2;

            int colorY = Y[yIndex];
            int colorV = UV[uvOffset];
            int colorU = UV[uvOffset + 1];

            int colorR = (int)(colorY + 1.370705 * (colorV - 128));
            int colorG = (int)(colorY - 0.698001 * (colorV - 128) - 0.337633 * (colorU - 128));
            int colorB = (int)(colorY + 1.732446 * (colorU - 128));

            colorR = qBound(0, colorR, 255);
            colorG = qBound(0, colorG, 255);
            colorB = qBound(0, colorB, 255);

            int pixelIndex = yIndex * 4;

            rgb[pixelIndex + 0] = colorB;
            rgb[pixelIndex + 1] = colorG;
            rgb[pixelIndex + 2] = colorR;
            rgb[pixelIndex + 3] = 0xff;
            //
        }
    }
}

void NV12_T_RGB(const unsigned char* Y, unsigned char* rgb,unsigned int width, unsigned int height)
{
    const unsigned char* UV = Y + width*height;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int yIndex = y * width + x;
            int uvOffset = (y / 2) * width + (x / 2) * 2;

            int colorY = Y[yIndex];
            int colorU = UV[uvOffset];
            int colorV = UV[uvOffset + 1];

            int colorR = (int)(colorY + 1.370705 * (colorV - 128));
            int colorG = (int)(colorY - 0.698001 * (colorV - 128) - 0.337633 * (colorU - 128));
            int colorB = (int)(colorY + 1.732446 * (colorU - 128));

            colorR = qBound(0, colorR, 255);
            colorG = qBound(0, colorG, 255);
            colorB = qBound(0, colorB, 255);

            int pixelIndex = yIndex * 4;

            rgb[pixelIndex + 0] = colorB;
            rgb[pixelIndex + 1] = colorG;
            rgb[pixelIndex + 2] = colorR;
            rgb[pixelIndex + 3] = 0xff;
            //
        }
    }
}

void QtRenderView::updatePlanes(const unsigned char* buf,int w, int h, int fourcc, int label)
{
    //0 yuv,1 nv12,2 nv21,3 yuyv,4 gb32
    _mutex.lock();
    if (_w != w || _h != h) {
        if (_img) {
            delete _img;
            _img = nullptr;
        }
    }
    if (!_img) {
        _img = new QImage(w, h, QImage::Format_RGB32);
    }
    _w = w;
    _h = h;
    _label = label;
    if (fourcc == 0) {
        yuv420_2_rgb8888(_img->bits(), buf, buf + w * h, buf + w * h * 5 / 4, w, h);
    }else if (fourcc == 1){
        NV12_T_RGB(buf,_img->bits(),w,h);
    }else if (fourcc == 2){
        NV21_T_RGB(buf,_img->bits(),w,h);
    }
    else if (fourcc == 3) {
        BlitYUY2((void*)buf, _img->bits(), w, h, w);
    }
    else if (fourcc == 4) {
        memcpy(_img->bits(),buf,w*h*4);
    }

    _mutex.unlock();
    emit onNeedUpdate();
}
void QtRenderView::AdjustPicture(int w, int h, int cx, int cy, int* iLeft, int* iTop, int* iWidth, int* iHeight)
{
    if(cap_view){
        // 100 -> 300 == 300  3√
        // 200 -> 300 == 900  1.5 (top = -300) 600 = 200*3  , -300 = (300 - 900)/2

        // 900 -> 300 == 450 0.3 (left = -75) 450 = 900*0.5 , -75 = (300-450)/2
        // 600 -> 300 == 300 0.5 √
        float fw = w*1.0/cx;
        float fh = h*1.0/cy;
        //qDebug()<<__func__<<fw<<fh;
        if(fw < fh){
            //qDebug()<<__func__<<"h";
            *iHeight = h;
            *iWidth = cx*fh;

            *iTop = 0;
            *iLeft = (w - *iWidth)/2;
        }else{
            //qDebug()<<__func__<<"w";
            *iHeight = cy*fw;
            *iWidth = w;

            *iTop = (h - *iHeight)/2;
            *iLeft = 0;
        }

    }else{
        float pic = cx * 1.0 / cy;
        float stage = w * 1.0 / h;
        if (pic >= stage) {
            //左右先顶到
            *iWidth = w;
            *iHeight = w / pic;
            *iLeft = 0;
            *iTop = (h - *iHeight) / 2;
        }
        else {
            //上下先顶到
            *iHeight = h;
            *iWidth = h * pic;
            //变成偶数
            if ((*iWidth) & 1) {
                (*iWidth)++;
            }
            *iTop = 0;
            *iLeft = (w - *iWidth) / 2;
        }
    }
}

#define VIDEO_CAMERA_0        0x434d4130
#define VIDEO_CAMERA_90       0x434d4139
#define VIDEO_CAMERA_180      0x434d4131
#define VIDEO_CAMERA_270      0x434d4132

#define VIDEO_ESCREEN_0       0x45534e30
#define VIDEO_ESCREEN_90      0x45534e39
#define VIDEO_ESCREEN_180     0x45534e31
#define VIDEO_ESCREEN_270     0x45534e32

#define VIDEO_SCREEN_0        0x53454e30
#define VIDEO_SCREEN_90       0x53454e39
#define VIDEO_SCREEN_180      0x53454e31
#define VIDEO_SCREEN_270      0x53454e32
#include <QPainter>
void QtRenderView::paintEvent(QPaintEvent* event)
{

    _mutex.lock();
    if (_need_flash) {
        QPainter painter(this);
        painter.fillRect(rect(), QBrush(QColor(Flash_Rgb[0], Flash_Rgb[1], Flash_Rgb[1])));
        _need_flash = false;
    }
    else if (_img) {
        QPainter painter(this);
        painter.fillRect(rect(), QBrush(QColor(0x00, 0x00, 0x00)));

        int morror = 0;
        int roate = 0;
        //int mirror = 0;//0 不镜像，1上下，2左右
        //int rotate = 0;//0，0度，1，90度，2，180，3，270度
        if(custom_matrix){
            morror = _morror;
            roate = _rotate;
        }else{
            switch (_label)
            {
            case 0:break;//本地共享屏幕
            case -1:break;//本地摄像头
            case VIDEO_CAMERA_0:break;
            case VIDEO_CAMERA_90:morror = 1; roate = 270; break;
            case VIDEO_CAMERA_180:roate = 180; break;
            case VIDEO_CAMERA_270:roate = 270; break;
            case VIDEO_ESCREEN_0:
            case VIDEO_SCREEN_0:
                break;
            case VIDEO_ESCREEN_90:
            case VIDEO_SCREEN_90:
                roate = 90;
                break;
            case VIDEO_ESCREEN_180:
            case VIDEO_SCREEN_180:
                break;
            case VIDEO_ESCREEN_270:
            case VIDEO_SCREEN_270:
                roate = 270; break;
            }
        }
        int l, t, r, b;
        if (roate == 270 || roate == 90) {
            AdjustPicture(this->width(), this->height(), _h, _w, &l, &t, &r, &b);
        }
        else {
            AdjustPicture(this->width(), this->height(), _w, _h, &l, &t, &r, &b);
        }
        if (morror && roate) {
            QMatrix matrix;
            matrix.rotate(roate);
            _img3 = _img->mirrored(morror & 0x2, morror&0x1).transformed(matrix, Qt::FastTransformation);
        }
        else if(morror){
            _img3 = _img->mirrored(morror & 0x2, morror & 0x1);
        }
        else if (roate) {
            QMatrix matrix;
            matrix.rotate(roate);
            _img3 = _img->transformed(matrix, Qt::FastTransformation);
        }
        else {
            _img3 = _img->copy(_img->rect());
        }
        painter.drawImage(QRect(l, t, r, b), _img3.scaled(r, b, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }else{

        QPainter painter(this);
        painter.fillRect(rect(), QBrush(QColor(0, 0, 0)));
    }
    _mutex.unlock();
}
