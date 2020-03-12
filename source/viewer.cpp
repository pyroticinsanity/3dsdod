/****************************************
Daggorath PC-Port Version 0.2.1
Richard Hunerlach
November 13, 2002

The copyright for Dungeons of Daggorath
is held by Douglas J. Morgan.
(c) 1982, DynaMicro
*****************************************/

// Dungeons of Daggorath
// PC-Port
// Filename: viewer.cpp
//
// Implementation of the Viewer class

#include "viewer.h"
#include "oslink.h"
#include "player.h"
#include "sched.h"
#include "parser.h"
#include "object.h"
#include "dungeon.h"
#include "creature.h"
#include "enhanced.h"


extern Creature		creature;
extern Dungeon		dungeon;
extern Object		object;
extern Parser		parser;
extern Coordinate	crd;
extern OS_Link		oslink;
extern Player		player;
extern Scheduler	scheduler;

// Constructor
Viewer::Viewer() : VCNTRX(128), VCNTRY(76),
				   fadChannel(3), buzzStep(300), midPause(2500),
				   prepPause(2500)
{
	Utils::LoadFromDecDigit(A_VLA, "411212717516167572757582823535424");
	Utils::LoadFromDecDigit(B_VLA, "6112128182151522224545525275758285262645455656757");
	Utils::LoadFromDecDigit(C_VLA, "51222271721515222275758285262635356666757");
	Utils::LoadFromDecDigit(D_VLA, "411515212175758181222271752626757");
	Utils::LoadFromDecDigit(E_VLA, "411212818245455252767682821616222");
	Utils::LoadFromDecDigit(F_VLA, "3112128182454552527676828");
	Utils::LoadFromDecDigit(G_VLA, "6122227172767682856666757216162225262635343636444");
	Utils::LoadFromDecDigit(H_VLA, "3112128185161685824545525");
	Utils::LoadFromDecDigit(I_VLA, "3215152222757582832424737");
	Utils::LoadFromDecDigit(J_VLA, "3122223132151522252626858");
	Utils::LoadFromDecDigit(K_VLA,
		"81121281824343525354546364656574757676858516162524252534333434434");
	Utils::LoadFromDecDigit(L_VLA, "21121281821616222");
	Utils::LoadFromDecDigit(M_VLA, "51121281851616858263637274656574733434636");
	Utils::LoadFromDecDigit(N_VLA, "51121281851616858253536263444453543535444");
	Utils::LoadFromDecDigit(O_VLA, "412222717526267572151522227575828");
	Utils::LoadFromDecDigit(P_VLA, "411212818245455255565675727575828");
	Utils::LoadFromDecDigit(Q_VLA,
		"712222717214142222757582833434434425253435161625253636757");
	Utils::LoadFromDecDigit(R_VLA,
		"711212818275758282454552555656757334344344252534351616252");
	Utils::LoadFromDecDigit(S_VLA,
		"712222313566667572151522224545525275758281525271752626454");
	Utils::LoadFromDecDigit(T_VLA, "416262818566668582757582831414737");
	Utils::LoadFromDecDigit(U_VLA, "3122228185262685821515222");
	Utils::LoadFromDecDigit(V_VLA, "51525281855656858233335254353554531414333");
	Utils::LoadFromDecDigit(W_VLA, "51121281851616858223233234252534333434535");
	Utils::LoadFromDecDigit(X_VLA,
		"9112123135161635316262818566668582333342443535444344445352535362645555646");
	Utils::LoadFromDecDigit(Y_VLA, "53141453525353626455556461626281856666858");
	Utils::LoadFromDecDigit(Z_VLA,
		"711616212176768181222231323333424344445354555564656666757");
	Utils::LoadFromDecDigit(NM0_VLA, "422323727373858575767625231515232");
	Utils::LoadFromDecDigit(NM1_VLA, "3316162324252584836464737");
	Utils::LoadFromDecDigit(NM2_VLA, "6116162121222241424545525556567575758282716262717");
	Utils::LoadFromDecDigit(NM3_VLA, "721515222122223135262645454552524556567575758282727261617");
	Utils::LoadFromDecDigit(NM4_VLA, "441515848646515142535362636464737");
	Utils::LoadFromDecDigit(NM5_VLA, "6122223132151522252626555555616151626271717676818");
	Utils::LoadFromDecDigit(NM6_VLA, "52151522252626454122227172454552527575828");
	Utils::LoadFromDecDigit(NM7_VLA, "6112123132333342434444535455556465666675717676818");
	Utils::LoadFromDecDigit(NM8_VLA, "721515222122224145262645424545525152527175565675727575828");
	Utils::LoadFromDecDigit(NM9_VLA, "6114142124252534353636757245455251525271727575828");
	Utils::LoadFromDecDigit(PER_VLA, "131414232");
	Utils::LoadFromDecDigit(UND_VLA, "111616212");
	Utils::LoadFromDecDigit(EXP_VLA, "23141423234444838");
	Utils::LoadFromDecDigit(QSM_VLA, "6314142323343453544545545556567571626271727575828");
	Utils::LoadFromDecDigit(SHL_VLA, "174848676");
	Utils::LoadFromDecDigit(SHR_VLA, "403131707233337271222261634444636");
	Utils::LoadFromDecDigit(LHL_VLA, "26474766673838777");
	Utils::LoadFromDecDigit(LHR_VLA, "50212170711212616223237273343473744545646");
	Utils::LoadFromDecDigit(FSL_VLA, "51121231323333424344445354555564656666858");
	Utils::LoadFromDecDigit(BSL_VLA, "51626281825353626344445354353544451616353");
	Utils::LoadFromDecDigit(PCT_VLA, "711212313233334243444453545555646566668581636381841616343");
	Utils::LoadFromDecDigit(PLS_VLA, "23242473714646515");
	Utils::LoadFromDecDigit(DSH_VLA, "114646515");

	AZ_VLA[1] = A_VLA;
	AZ_VLA[2] = B_VLA;
	AZ_VLA[3] = C_VLA;
	AZ_VLA[4] = D_VLA;
	AZ_VLA[5] = E_VLA;
	AZ_VLA[6] = F_VLA;
	AZ_VLA[7] = G_VLA;
	AZ_VLA[8] = H_VLA;
	AZ_VLA[9] = I_VLA;
	AZ_VLA[10] = J_VLA;
	AZ_VLA[11] = K_VLA;
	AZ_VLA[12] = L_VLA;
	AZ_VLA[13] = M_VLA;
	AZ_VLA[14] = N_VLA;
	AZ_VLA[15] = O_VLA;
	AZ_VLA[16] = P_VLA;
	AZ_VLA[17] = Q_VLA;
	AZ_VLA[18] = R_VLA;
	AZ_VLA[19] = S_VLA;
	AZ_VLA[20] = T_VLA;
	AZ_VLA[21] = U_VLA;
	AZ_VLA[22] = V_VLA;
	AZ_VLA[23] = W_VLA;
	AZ_VLA[24] = X_VLA;
	AZ_VLA[25] = Y_VLA;
	AZ_VLA[26] = Z_VLA;
	AZ_VLA[27] = PER_VLA;
	AZ_VLA[28] = UND_VLA;
	AZ_VLA[29] = EXP_VLA;
	AZ_VLA[30] = QSM_VLA;
	AZ_VLA[31] = SHL_VLA;
	AZ_VLA[32] = SHR_VLA;
	AZ_VLA[33] = LHL_VLA;
	AZ_VLA[34] = LHR_VLA;
	AZ_VLA[35] = NM0_VLA;
	AZ_VLA[36] = NM1_VLA;
	AZ_VLA[37] = NM2_VLA;
	AZ_VLA[38] = NM3_VLA;
	AZ_VLA[39] = NM4_VLA;
	AZ_VLA[40] = NM5_VLA;
	AZ_VLA[41] = NM6_VLA;
	AZ_VLA[42] = NM7_VLA;
	AZ_VLA[43] = NM8_VLA;
	AZ_VLA[44] = NM9_VLA;
	AZ_VLA[45] = FSL_VLA;
	AZ_VLA[46] = BSL_VLA;
	AZ_VLA[47] = PCT_VLA;
	AZ_VLA[48] = PLS_VLA;
	AZ_VLA[49] = DSH_VLA;

	Utils::LoadFromHex(SP_VLA,
		(const char*)"020BA07CA474A878B074B878B07CA878B078B878BC74C07C07A87CAA74AC78B07AB478"
		"B674B87C");

	Utils::LoadFromHex(WR_VLA,
		(const char*)"0303443E58446438095A4A4A46504C5A4A644866405A4A5E565A56075064565A645C6C"
		"62625E565E5064");

	Utils::LoadFromHex(SC_VLA,
		(const char*)"020D4A704A6C46683E6C42744A765278527C4A7C427446783E78427C065A7C5A785278"
		"4A7C4A805280");
	Utils::LoadFromHex(BL_VLA,
		(const char*)"0314825272566C646A6E6A7860826A806884727E80808E82948098849A80A2829C7C9A"
		"6C945C8A568252088256885C8A6280648256785C7666806403746C76729078");
	Utils::LoadFromHex(GL_VLA,
		(const char*)"04157C50725E786E70844E6830844844205458167234805C8E34A816E058B844D084B2"
		"709084886E8E5E84500970847A7C7E6E7A64805C8664826E867C90840B7A526A605C5C"
		"564C4E48544E4C4E54525860666874641BA816A21AA612A018A01E96268C2E862A8A20"
		"8412862080267A207C1276207A2A8036862A802E7A2A742E6A26601E60185A125E1A5816");
	Utils::LoadFromHex(VI_VLA,
		(const char*)"020F82847A707C5C7E5E825E845C82708C80888472846C78766A7078747C7E7C107864"
		"78607C5C785878547A527E567A5286528256865288548858845C88608864");
	Utils::LoadFromHex(S1_VLA,
		"050A62687062785C805E846084667E6878687266646A0C846688669470A07AAE7CAA78"
		"AE78A274986A8E60825C7658085C4E604662505E565C4E544C58545E560B5A6A566C58"
		"6E5E6C6278547E587A507A58764C6650621554565C60666C627056684A5E4C52544C50"
		"48544A58445E46604260466C44744878566860645C6E546A4A");
	Utils::LoadFromHex(S2_VLA,
		"050462687C5E7E60646A108466725C7666726E846688669470A07AAE7CAA78AE78A274"
		"986A8E60825C7658085C4E604662505E565C4E544C58545E560B5A6A566C586E5E6C62"
		"78547E587A507A58764C6650621554565C60666C627056684A5E4C52544C5048544A58"
		"445E46604260466C44744878566860645C6E546A4A");
	Utils::LoadFromHex(K1_VLA,
		"08057C22842282247E247C22088E508840922E9C408C52884C92408C3A068C509880A0"
		"849084907E8254067E546E7E6E845C8466807450118C50805672507840743A6E2E7A2A"
		"7C1E801A841E862A922E862A862E80347A2E7A2A07803480147C147E18821884148014"
		"0E664A66466A466A405A405A465E465E4A604A60506450644A664A743A096E2E664064"
		"40661E62145E1E604062406214");
	Utils::LoadFromHex(K2_VLA,
		"08047E1E7E287C287C2407962CA634A44C965C884C8634962C068C509880A084908490"
		"7E8254067E546E7E6E845C8466807450118C50805672507840743A6E2E7A2A7C1E801A"
		"841E862A922E862A862E80347A2E7A2A07803480147C147E188218841480140E664A66"
		"466A466A405A405A465E465E4A604A60506450644A664A743A096E2E66406440661E62"
		"145E1E604062406214");
	Utils::LoadFromHex(W0_VLA,
		"080D7C40784878406E4E66406E3A763678327A2E74286C2A781C7E1C08821C881C9224"
		"9432882C842E8630882C0986308E36A474848476825E786E5A84846A4805664064425E"
		"38603666400566426244664A684C6E5A027058784809843E80147A347A407C3C807282"
		"508244843E0B822880267C287E2A8228802E80327E327E2A7C287E2E");
	Utils::LoadFromHex(W1_VLA,
		"0A0D7C40784878406E4E66406E3A763678327A2E74286C2A781C7E1C08821C881C9224"
		"9432882C842E8630882C0986308E36A474848476825E786E5A84846A4805664064425E"
		"38603666400566426244664A684C6E5A027058784809843E80147A347A407C3C807282"
		"508244843E0B822880267C287E2A8228802E80327E327E2A7C287E2E10622E64326236"
		"5C3A5638523056285A265E285C28582A563058345C366232622E109A689C6C9A709474"
		"8E728A6A8E6292609662946290648E6A926E96709A6C9A68");
	Utils::LoadFromHex(W2_VLA,
		"0C0D7C40784878406E4E66406E3A763678327A2E74286C2A781C7E1C08821C881C9224"
		"9432882C842E8630882C0986308E36A474848476825E786E5A84846A4805664064425E"
		"38603666400566426244664A684C6E5A027058784809843E80147A347A407C3C807282"
		"508244843E0B822880267C287E2A8228802E80327E327E2A7C287E2E0656285C40642A"
		"523668385628068C428C508644904A864C8C42069260947888649A6A8A749260067450"
		"7A5A72567A52745A7450");
	Utils::LoadFromHex(LAD_VLA,
		"0B0274187480028C188C8002741C8C1C0274288C280274348C340274408C4002744C8C"
		"4C0274588C580274648C640274708C7002747B8C7B");
	Utils::LoadFromHex(HUP_VLA,
		"040664225C18A4189C2264226418029C229C18022F1C601C02A11CD21C");
	Utils::LoadFromHex(HDN_VLA,  "020664765C80A4809C7664766480029C769C80");
	Utils::LoadFromHex(CEI_VLA,  "01022F1CD11C");
	Utils::LoadFromHex(LPK_VLA,  "01051C64246C207024781C80");
	Utils::LoadFromHex(RPK_VLA,  "0105E464DC6CE070DC78E480");
	Utils::LoadFromHex(FSD_VLA,  "01036C7180439472");
	Utils::LoadFromHex(LSD_VLA,  "0103288032423A75");
	Utils::LoadFromHex(RSD_VLA,	 "0103D880CE42C675");
	Utils::LoadFromHex(RWAL_VLA, "0104E510C026C072E588");
	Utils::LoadFromHex(LWAL_VLA, "01041B10402640721B88");
	Utils::LoadFromHex(FWAL_VLA, "02024026C026024072C072");
	Utils::LoadFromHex(RPAS_VLA, "0204E526C026C072E57202E510C026");
	Utils::LoadFromHex(LPAS_VLA, "02041D26402640721B72021B104026");
	Utils::LoadFromHex(FPAS_VLA, "00");
	Utils::LoadFromHex(RDOR_VLA,
		"0304E510C026C072E58804D880D841C844C87702D05CCC5D");
	Utils::LoadFromHex(LDOR_VLA,
		"03041B10402640721B8804288028413844387702305C345D");
	Utils::LoadFromHex(FDOR_VLA,
		"04024026C026024072C072046C726C4394439472027E5E825E");
	Utils::LoadFromHex(SHIE_VLA, "0106AC86C080BA7AA880A486AC86");
	Utils::LoadFromHex(SWOR_VLA, "02025072647C0252765672");
	Utils::LoadFromHex(TORC_VLA, "01043C764A7448723C76");
	Utils::LoadFromHex(RING_VLA, "01053C7A3E7C3C7E3A7C3C7A");
	Utils::LoadFromHex(SCRO_VLA, "0105C276C078C87ECA7CC276");
	Utils::LoadFromHex(FLAS_VLA, "0104A26EA478A078A26E");

	TXTEXA.SetVals(examArea, 0, 32 * 19, 0);
	TXTPRI.SetVals(textArea, 0, 32 * 4, 20);
	TXTSTS.SetVals(statArea, 0, 32 * 1, 19);

	LArch[0] = LPAS_VLA;
	LArch[1] = LDOR_VLA;
	LArch[2] = LSD_VLA;
	LArch[3] = LWAL_VLA;

	FArch[0] = FPAS_VLA;
	FArch[1] = FDOR_VLA;
	FArch[2] = FSD_VLA;
	FArch[3] = FWAL_VLA;

	RArch[0] = RPAS_VLA;
	RArch[1] = RDOR_VLA;
	RArch[2] = RSD_VLA;
	RArch[3] = RWAL_VLA;

	FLATAB[0] = 3;
	FLATAB[1] = 0;
	FLATAB[2] = 1;

	FLATABv[0] = LArch;
	FLATABv[1] = FArch;
	FLATABv[2] = RArch;

	FWDOBJ[0] = FLAS_VLA;
	FWDOBJ[1] = RING_VLA;
	FWDOBJ[2] = SCRO_VLA;
	FWDOBJ[3] = SHIE_VLA;
	FWDOBJ[4] = SWOR_VLA;
	FWDOBJ[5] = TORC_VLA;

	FWDCRE[0] = SP_VLA;
	FWDCRE[1] = VI_VLA;
	FWDCRE[2] = S1_VLA;
	FWDCRE[3] = BL_VLA;
	FWDCRE[4] = K1_VLA;
	FWDCRE[5] = S2_VLA;
	FWDCRE[6] = SC_VLA;
	FWDCRE[7] = K2_VLA;
	FWDCRE[8] = WR_VLA;
	FWDCRE[9] = GL_VLA;
	FWDCRE[10] = W0_VLA;
	FWDCRE[11] = W1_VLA;

	float start1=200.0f;
	float start2=256.0f;
	for (int x=0;x<9;x++) {
		Scalef[x]=start1;
		Scalef[x+10]=start2;
		start1*=0.633f;
		start2*=0.633f;
	}
	Scalef[20]=start2;

	Utils::LoadFromHex(Scale, "C88050321F140C080402FF9C6441281A100A060301");
	Utils::LoadFromHex(enough1, "FFC0573EA746C0905132281E6051099820C0E7DEF0");
	Utils::LoadFromHex(enough2, "E8000848B00C8A0A3C0D29680A232023DEDDEF60");
	Utils::LoadFromHex(winner1, "FFC4543D84D80859D12EC80370A693051050202E20");
	Utils::LoadFromHex(winner2, "C80000000003CC0081C5B82E9D0644F7BC");
	Utils::LoadFromHex(death, "FFC192D00173E882C8047966073E809169593BDEF0");
	Utils::LoadFromHex(copyright, "F8DF0CC92745000265C10352393C0068DACC630948");
	Utils::LoadFromHex(welcome1, "9FD2020645064A02BA8597BDEF80");
	Utils::LoadFromHex(welcome2, "F7BDEA20A0255C72BDD303CC0204E77C83446F7B");
	Utils::LoadFromHex(prepstr, "3C24580645D8");
	Utils::LoadFromHex(exam1, "625C0A2133049EF6FC");
	Utils::LoadFromHex(exam2, "56C72286959177F0");
	Utils::LoadFromHex(exam3, "408235C0235FC0");
	Utils::LoadFromHex(exps, "16F7B0");
	Utils::LoadFromHex(LINES, "02020010FF10020088FF88");

	//Reset();		//Can't call this before OpenGL initialization?!?!
}

void Viewer::Reset()
{
	showSeerMap = true;
	setVidInv(false);
	UPDATE = 0;
	display_mode = MODE_TITLE;
	HLFSTP = 0;
	BAKSTP = 0;
	MAGFLG = 0;
	PASFLG = 0;
	NORSCL = 0;
	HLFSCL = 10;
	BAKSCL = 11;
	TXBFLG = 0;
	TXB_U = 0;
	tcaret = 0;
	tlen = 0;
	RLIGHT = 0;
	MLIGHT = 0;
	VCTFAD = 32;
	delay = 0;
	done = false;
	fadeVal = -2;
	clearArea(&TXTPRI);
	clearArea(&TXTEXA);
	clearArea(&TXTSTS);
}

// Public Interface
void Viewer::initialize()
{
	_renderer = RendererFactory::GetRenderer();
	_renderer->setClearColor(bgColor[0], bgColor[1], bgColor[2]);
	
	_renderer->setViewport(0, 0, _renderer->getScreenWidth(), _renderer->getScreenHeight());
	_renderer->initialize();
}

void Viewer::setVidInv(bool inv)
{
	if (!inv)
	{
		// white on black
		bgColor[0] = 0.0;
		bgColor[1] = 0.0;
		bgColor[2] = 0.0;

		fgColor[0] = 1.0;
		fgColor[1] = 1.0;
		fgColor[2] = 1.0;
	}
	else
	{
		// black on white
		bgColor[0] = 1.0;
		bgColor[1] = 1.0;
		bgColor[2] = 1.0;

		fgColor[0] = 0.0;
		fgColor[1] = 0.0;
		fgColor[2] = 0.0;
	}
	_renderer->setClearColor(bgColor[0], bgColor[1], bgColor[2]);
}

// This is the main renderer routine.  It draws either
// the map, or the 3D/Examine-Status-Text Area.
void Viewer::draw_game()
{
	if (UPDATE == 0)
	{
		return;
	}
	if (display_mode == MODE_MAP)
	{
		// Draw Map
		_renderer->setClearColor(1.0, 1.0, 1.0, 1.0);
		_renderer->clearBuffer();
		_renderer->setClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0);
		_renderer->resetMatrix();
		MAPPER();
		_renderer->swapBuffers();
	}
	else if(display_mode == MODE_COMMAND_CREATOR)
	{
		//drawCommandMenu(commandMenu, col, row);
	}
	else if(display_mode == MODE_KEYBOARD)
	{

	}
	else
	{
		// Draw View Port (3D or Examine or Prepare!)
		_renderer->clearBuffer();

		_renderer->resetMatrix();
		_renderer->setColor(fgColor);
		switch (display_mode)
		{
		case MODE_3D:
			VIEWER();
			break;
		case MODE_EXAMINE:
			clearArea(&TXTEXA);
			EXAMIN();
			drawArea(&TXTEXA);
			if (player.PTORCH != -1)
			{
				drawTorchHighlite();
			}
			break;
		case MODE_TITLE:
			drawArea(&TXTEXA);
			break;
		default:
			break;
		}

		// Draw Status Line
		drawArea(&TXTSTS);

		// Draw Text Area
		drawArea(&TXTPRI);

		_renderer->swapBuffers();
	}
	UPDATE = 0;
}

// This method renders the wizard fade in/out animations.
// The parameter fadeMode indicates which of the four fades
// to do:
//   1 = Beginning before game starts
//   2 = Intermission after killing wizards image
//   3 = Death
//   4 = Victory
bool Viewer::ShowFade(int fadeMode)
{
	Uint32 ticks1, ticks2;
	SDL_Event event;
	int * wiz;
	VXSCAL = 0x80;
	VYSCAL = 0x80;

	clearArea(&TXTPRI);

	switch (fadeMode)
	{
	case 1:
		wiz = W1_VLA;
		displayCopyright();
		displayWelcomeMessage();
		break;
	case 2:
		wiz = W1_VLA;
		clearArea(&TXTSTS);
		displayEnough();
		break;
	case 3:
		wiz = W1_VLA;
		clearArea(&TXTSTS);
		displayDeath();
		break;
	case 4:
		wiz = W2_VLA;
		clearArea(&TXTSTS);
		displayWinner();
	}

	while(SDL_PollEvent(&event)) ; // clear event buffer

	RANGE=1;
	SETSCL();

	// Start buzz
	Mix_Volume(fadChannel, 0);
	Mix_PlayChannel(fadChannel, creature.buzz, -1);

	for (VCTFAD = 32; (VCTFAD & 128) == 0; VCTFAD -= 2)
	{
		// Set volume of buzz
		Mix_Volume(fadChannel, ((32 - VCTFAD) / 2) * (oslink.volumeLevel / 16) );

		_renderer->clearBuffer();
		_renderer->resetMatrix();
		drawArea(&TXTSTS);
		_renderer->setColor(fgColor);
		_renderer->resetMatrix();
		drawVectorList(wiz);
		_renderer->swapBuffers();

		ticks1 = SDL_GetTicks();
		do
		{
			ticks2 = SDL_GetTicks();
			if (fadeMode == 1 && scheduler.keyCheck())
			{
				Mix_HaltChannel(fadChannel);
				clearArea(&TXTPRI);
				while(SDL_PollEvent(&event)) ; // clear event buffer
				return false;
			}
		} while (ticks2 < ticks1 + buzzStep);
	}

	VCTFAD = 0;

	// do crash
	Mix_HaltChannel(fadChannel);
	Mix_Volume(fadChannel, oslink.volumeLevel);
	Mix_PlayChannel(fadChannel, creature.kaboom, 0);
	while (Mix_Playing(fadChannel) == 1)
	{
		if (fadeMode == 1 && scheduler.keyCheck())
		{
			Mix_HaltChannel(fadChannel);
			clearArea(&TXTPRI);
			while(SDL_PollEvent(&event)) ; // clear event buffer
			return false;
		}
	}

	// show message
	_renderer->clearBuffer();
	_renderer->resetMatrix();
	drawArea(&TXTSTS);
	_renderer->setColor(fgColor);
	_renderer->resetMatrix();
	drawVectorList(wiz);
	drawArea(&TXTPRI);
	_renderer->swapBuffers();

	if (fadeMode < 3)
	{
		// pause with wiz, status, and message
		ticks1 = SDL_GetTicks();
		do
		{
			ticks2 = SDL_GetTicks();

			_renderer->clearBuffer();
			_renderer->resetMatrix();
			drawArea(&TXTSTS);
			_renderer->setColor(fgColor);
			_renderer->resetMatrix();
			drawVectorList(wiz);
			drawArea(&TXTPRI);
			_renderer->swapBuffers();

			if (fadeMode != 2 && scheduler.keyCheck())
			{
				clearArea(&TXTPRI);
				while(SDL_PollEvent(&event)) ; // clear event buffer
				return false;
			}
		} while (ticks2 < ticks1 + midPause);

		// erase message
		_renderer->clearBuffer();
		_renderer->resetMatrix();
		drawArea(&TXTSTS);
		_renderer->setColor(fgColor);
		_renderer->resetMatrix();
		drawVectorList(wiz);
		_renderer->swapBuffers();

		// do crash
		Mix_PlayChannel(fadChannel, creature.kaboom, 0);
		while (Mix_Playing(fadChannel) == 1)
		{
			if (fadeMode != 2 && scheduler.keyCheck())
			{
				Mix_HaltChannel(fadChannel);
				clearArea(&TXTPRI);
				while(SDL_PollEvent(&event)) ; // clear event buffer
				return false;
			}
		}

		// start buzz again
		Mix_Volume(fadChannel, 0);
		Mix_PlayChannel(fadChannel, creature.buzz, -1);

		for (VCTFAD = 0; VCTFAD <= 32; VCTFAD += 2)
		{
			// Set volume of buzz
			Mix_Volume(fadChannel, ((32 - VCTFAD) / 2) * (oslink.volumeLevel / 16) );

			_renderer->clearBuffer();
			_renderer->resetMatrix();
			drawArea(&TXTSTS);
			_renderer->setColor(fgColor);
			_renderer->resetMatrix();
			drawVectorList(wiz);
			_renderer->swapBuffers();

			ticks1 = SDL_GetTicks();
			do
			{
				ticks2 = SDL_GetTicks();
				if (fadeMode != 2 && scheduler.keyCheck())
				{
					Mix_HaltChannel(fadChannel);
					clearArea(&TXTPRI);
					while(SDL_PollEvent(&event)) ; // clear event buffer
					return false;
				}
			} while (ticks2 < ticks1 + buzzStep);
		}
	}

	Mix_HaltChannel(fadChannel);

	if (fadeMode < 3)
	{
		clearArea(&TXTPRI);
		while(SDL_PollEvent(&event)) ; // clear event buffer
		return true;
	}
	else
	{
		while (!scheduler.keyCheck()) // Wait for a key
		  {
			_renderer->clearBuffer();
			_renderer->resetMatrix();
			drawArea(&TXTSTS);
			_renderer->setColor(fgColor);
			_renderer->resetMatrix();
			drawVectorList(wiz);
			drawArea(&TXTPRI);
			_renderer->swapBuffers();
		  }
		clearArea(&TXTPRI);
		while(SDL_PollEvent(&event)) ; // clear event buffer
		return false;
	}
}

// This is the renderer method used to do the wizard
// fade in/out.  It's only used during the opening.  It
// is syncronized with the 30Hz buzz and the wizard
// crashing sound.
bool Viewer::draw_fade()
{
	delay1 = delay2 = SDL_GetTicks();

	if ((!done && delay1 > delay + buzzStep) && fadeVal != 0)
	{
		_renderer->clearBuffer();
	}
	_renderer->resetMatrix();

	drawArea(&TXTSTS);

	_renderer->setColor(fgColor);

	if ((!done && delay1 > delay + buzzStep) && fadeVal != 0)
	{
		// Set volume of buzz
		Mix_Volume(fadChannel, ((32 - VCTFAD) / 2) * (oslink.volumeLevel / 16) );

		_renderer->resetMatrix();
		drawVectorList(W1_VLA);
		_renderer->swapBuffers();
		VCTFAD += fadeVal;
		if ((VCTFAD & 0x80) != 0)
		{
			// do sound crash
			Mix_HaltChannel(fadChannel);
			Mix_Volume(fadChannel, oslink.volumeLevel);
			Mix_PlayChannel(fadChannel, creature.kaboom, 0);
			while (Mix_Playing(fadChannel) == 1)
			{
				// Call keyboard routine ???
			}

			VCTFAD = 0;
			fadeVal = 0;
		}
		if (VCTFAD == 32 && fadeVal == 2)
		{
			done = true;
			clearArea(&TXTPRI);
			drawArea(&TXTPRI);
		}
		delay = SDL_GetTicks();
	}

	if (VCTFAD == 0 && fadeVal == 0)
	{
		_renderer->resetMatrix();
		drawVectorList(W1_VLA);
		drawArea(&TXTPRI);
		_renderer->swapBuffers();
		delay2 = SDL_GetTicks();
		if (delay2 > delay + midPause)
		{
			// do sound crash
			Mix_PlayChannel(fadChannel, creature.kaboom, 0);
			while (Mix_Playing(fadChannel) == 1)
			{
				// Call keyboard routine ???
			}

			fadeVal = 2;
			delay = SDL_GetTicks();

			Mix_PlayChannel(fadChannel, creature.buzz, -1);
		}
	}

	if (!done)
	{
		delay1 = delay2 = SDL_GetTicks();
	}
	return done;
}

// Same as above, but used for the intermission
void Viewer::enough_fade()
{
	delay1 = delay2 = SDL_GetTicks();

	if ((!done && delay1 > delay + buzzStep) && fadeVal != 0)
	{
		_renderer->clearBuffer();
	}
	_renderer->resetMatrix();

	drawArea(&TXTSTS);

	_renderer->setColor(fgColor);

	if ((!done && delay1 > delay + buzzStep) && fadeVal != 0)
	{
		// Set volume of buzz
		Mix_Volume(fadChannel, ((32 - VCTFAD) / 2) * (oslink.volumeLevel / 16) );

		_renderer->resetMatrix();
		drawVectorList(W1_VLA);
		_renderer->swapBuffers();
		VCTFAD += fadeVal;
		if ((VCTFAD & 0x80) != 0)
		{
			displayEnough();
			drawArea(&TXTPRI);
			_renderer->swapBuffers();

			// do sound crash
			Mix_HaltChannel(fadChannel);
			Mix_Volume(fadChannel, oslink.volumeLevel);
			Mix_PlayChannel(fadChannel, creature.kaboom, 0);
			while (Mix_Playing(fadChannel) == 1)
			{
				// Call keyboard routine ???
			}

			VCTFAD = 0;
			fadeVal = 0;
		}
		if (VCTFAD == 32 && fadeVal == 2)
		{
			done = true;
			clearArea(&TXTPRI);
			drawArea(&TXTPRI);
		}
		delay = SDL_GetTicks();
	}

	if (VCTFAD == 0 && fadeVal == 0)
	{
		_renderer->resetMatrix();
		drawVectorList(W1_VLA);
		VCTFAD += fadeVal;
		drawArea(&TXTPRI);
		_renderer->swapBuffers();
		delay2 = SDL_GetTicks();
		if (delay2 > delay + midPause)
		{
			// do sound crash
			Mix_PlayChannel(fadChannel, creature.kaboom, 0);
			while (Mix_Playing(fadChannel) == 1)
			{
				// Call keyboard routine ???
			}

			fadeVal = 2;
			delay = SDL_GetTicks();

			Mix_PlayChannel(fadChannel, creature.buzz, -1);
		}
	}

	if (!done)
	{
		delay1 = delay2 = SDL_GetTicks();
	}
}

// Same as above, but used for death & victory
void Viewer::death_fade(int WIZ[])
{
	delay1 = SDL_GetTicks();

	if ((delay1 > delay + buzzStep) && fadeVal != 0)
	{
		// Set volume of buzz
		Mix_Volume(fadChannel, ((32 - VCTFAD) / 2) * (oslink.volumeLevel / 16) );

		_renderer->clearBuffer();
		_renderer->resetMatrix();
		drawArea(&TXTSTS);
		_renderer->setColor(fgColor);
		_renderer->resetMatrix();
		drawVectorList(WIZ);
		_renderer->swapBuffers();
		VCTFAD += fadeVal;
		if ((VCTFAD & 0x80) != 0)
		{
			// do sound crash
			Mix_HaltChannel(fadChannel);
			Mix_Volume(fadChannel, oslink.volumeLevel);
			_renderer->resetMatrix();
			drawArea(&TXTPRI);
			_renderer->swapBuffers();
			Mix_PlayChannel(fadChannel, creature.kaboom, 0);
			while (Mix_Playing(fadChannel) == 1)
			{
				// Call keyboard routine ???
			}

			VCTFAD = 0;
			fadeVal = 0;
			done = true;
		}
		delay = SDL_GetTicks();
	}

	if (fadeVal == 0)
	{
		_renderer->resetMatrix();
		drawVectorList(WIZ);
		drawArea(&TXTPRI);
		_renderer->swapBuffers();
	}
}

void Viewer::displayCopyright()
{
	TXB_U = &TXTSTS;
	--TXBFLG;
	OUTSTI(copyright);
	TXBFLG = 0;
}

void Viewer::displayWelcomeMessage()
{
	OUTSTI(welcome1);
	OUTSTI(welcome2);
}

void Viewer::displayDeath()
{
	OUTSTI(death);
}

void Viewer::displayWinner()
{
	OUTSTI(winner1);
	OUTSTI(winner2);
}

void Viewer::displayEnough()
{
	OUTSTI(enough1);
	OUTSTI(enough2);
}

void Viewer::displayPrepare()
{
	clearArea(&TXTEXA);
	TXB_U = &TXTEXA;
	TXB_U->caret = (32*9)+12;
	--TXBFLG;
	OUTSTI(prepstr);
	TXBFLG = 0;
}

void Viewer::drawTorchHighlite()
{
	int x1, y1, x2, y2;
	x1 = tcaret - ((tcaret / 32) * 32);
	y1 = tcaret / 32;
	x2 = x1 + tlen;
	y2 = y1 + 1;
	_renderer->resetMatrix();
	_renderer->setColor(fgColor);
	_renderer->drawQuad(
		crd.newX(x1*8), crd.newY(y1*8),
		crd.newX(x2*8), crd.newY(y1*8),
		crd.newX(x2*8), crd.newY(y2*8),
		crd.newX(x1*8), crd.newY(y2*8));
	_renderer->setColor(bgColor);
	object.OBJNAM(player.PTORCH);
	drawString_internal(x1, y1, parser.TOKEN, tlen);
}

void Viewer::drawArea(TXB * a)
{
	int cnt = 0;

	if (a->top == 19)
	{
		_renderer->resetMatrix();
		_renderer->setColor(fgColor);
		_renderer->drawQuad(
			crd.newX(0*8), crd.newY(19*8),
			crd.newX(32*8), crd.newY(19*8),
			crd.newX(32*8), crd.newY(20*8),
			crd.newX(0*8), crd.newY(20*8));
		_renderer->setColor(bgColor);
	}
	else
	{
		_renderer->resetMatrix();
		_renderer->setColor(bgColor);
		_renderer->drawQuad(
			crd.newX(0*8), crd.newY(20*8),
			crd.newX(33*8), crd.newY(20*8),
			crd.newX(33*8), crd.newY(24*8),
			crd.newX(0*8), crd.newY(24*8));
		_renderer->setColor(fgColor);
	}

	while (cnt < a->len)
	{
		drawString(0, a->top + cnt / 32, a->area + cnt, 32);
		cnt += 32;
	}
}

void Viewer::clearArea(TXB * a)
{
	strcpy(a->area, "                                ");
	int cnt = 32;
	while (cnt < a->len)
	{
		strcat(a->area, "                                ");
		cnt += 32;
	}
	a->caret = 0;
}

// This method checks if the screen needs updated.
// Called from the scheduler every 3 tenths of a second.
int Viewer::LUKNEW()
{
	// Update Task's next_time
	scheduler.TCBLND[Scheduler::TID_REFRESH_DISP].next_time = scheduler.curTime +
		scheduler.TCBLND[Scheduler::TID_REFRESH_DISP].frequency;

	if (display_mode != MODE_MAP)
	{
		return 0;
	}

	NEWLUK = 0;
	PUPDAT();
	return 0;
}

// Updates the screen.
void Viewer::PUPDAT()
{
	if (player.FAINT != 0)
	{
		return;
	}

	PUPSUB();

	--UPDATE;
	draw_game();
}

// Sets lighting values.
void Viewer::PUPSUB()
{
	dodBYTE A, B;
	A = player.PRLITE;
	B = player.PMLITE;

	if (player.PTORCH != -1)
	{
		A += object.OCBLND[player.PTORCH].P_OCXX1;
		B += object.OCBLND[player.PTORCH].P_OCXX2;
	}
	RLIGHT = A;
	MLIGHT = B;
}

// Updates the Left and Right hand in the status line
void Viewer::STATUS()
{
	int	ctr, len, offset, idx;
	for (ctr = 0; ctr < 15; ++ctr)
	{
		statArea[ctr] = ' ';
		statArea[ctr + 17] = ' ';
	}
	idx = ((player.PLHAND & 0x8000) != 0) ? -1 : player.PLHAND;
	object.OBJNAM(idx);
	ctr = 0;
	while (parser.TOKEN[ctr] != 0xFF)
	{
		if (parser.TOKEN[ctr] == 0)
		{
			statArea[ctr] = ' ';
		}
		else
		{
			statArea[ctr] = parser.TOKEN[ctr] | 64;
		}
		++ctr;
	}
	idx = ((player.PRHAND & 0x8000) != 0) ? -1 : player.PRHAND;
	object.OBJNAM(idx);

	ctr = 0;
	while (parser.TOKEN[ctr] != 0xFF)
	{
		++ctr;
	}

	len = ctr;
	ctr = 32 - len;
	offset = ctr;
	while (ctr < 32)
	{
		if (parser.TOKEN[ctr - offset] == 0)
		{
			statArea[ctr] = ' ';
		}
		else
		{
			statArea[ctr] = parser.TOKEN[ctr - offset] | 64;
		}
		++ctr;
	}
}

void Viewer::OUTSTI(dodBYTE * comp)
{
	int c;
	parser.EXPAND(comp, &c, 0);
	OUTSTR(&parser.STRING[1]);
}

void Viewer::OUTSTR(dodBYTE * str)
{
	int ctr = 0;
	while (*(str + ctr) != 0xFF)
	{
		OUTCHR(*(str + ctr));
		++ctr;
	}
}

void Viewer::OUTCHR(dodBYTE c)
{
	if (TXBFLG == 0)
	{
		TXB_U = &TXTPRI;
	}

	TXTXXX(c);
	if (TXB_U->caret == TXB_U->len && TXB_U->top != 19)
	{
		TXTSCR();
	}
	--UPDATE;
}

void Viewer::TXTXXX(dodBYTE c)
{
	if (c == parser.I_BS)
	{
		// backspace
		if (TXB_U->caret > 0)
		{
			TXB_U->caret -= 1;
		}
		return;
	}
	if (c == parser.I_CR)
	{
		// carriage return
		TXB_U->caret += 32;
		TXB_U->caret = 32 * (TXB_U->caret / 32);
		return;
	}

	if (c == parser.I_SP)
	{
		TXB_U->area[TXB_U->caret] = ' ';
	}
	else if (c >= 1 && c <= 26)
	{
		TXB_U->area[TXB_U->caret] = (c | 64);
	}
	else if (c == parser.I_EXCL)
	{
		TXB_U->area[TXB_U->caret] = '!';
	}
	else if (c == parser.I_BAR)
	{
		TXB_U->area[TXB_U->caret] = '_';
	}
	else if (c == parser.I_QUES)
	{
		TXB_U->area[TXB_U->caret] = '?';
	}
	else if (c == parser.I_DOT)
	{
		TXB_U->area[TXB_U->caret] = '.';
	}
	else if (c == parser.I_SHL)
	{
		TXB_U->area[TXB_U->caret] = '<';
	}
	else if (c == parser.I_SHR)
	{
		TXB_U->area[TXB_U->caret] = '>';
	}
	else if (c == parser.I_LHL)
	{
		TXB_U->area[TXB_U->caret] = '{';
	}
	else if (c == parser.I_LHR)
	{
		TXB_U->area[TXB_U->caret] = '}';
	}

	++TXB_U->caret;
}

void Viewer::TXTSCR()
{
	int ctr;
	for (ctr = 0; ctr < TXB_U->len - 32; ++ctr)
	{
		TXB_U->area[ctr] = TXB_U->area[ctr + 32];
	}
	for (ctr = TXB_U->len - 32; ctr < TXB_U->len; ++ctr)
	{
		TXB_U->area[ctr] = 0;
	}
	TXB_U->caret = TXB_U->len - 32;
	if (player.PTORCH != -1 && TXB_U->len > 128)
	{
		tcaret -= 32;
	}
}

void Viewer::PROMPT()
{
	OUTSTR(parser.M_PROM1);
}

// This is the 3D-Viewport rendering routine
void Viewer::VIEWER()
{
	dodBYTE a, b, x, u, ftctr, vft;
	int creNum, objIdx;

	RANGE = 0;
	dungeon.DROW.setRC(player.PROW, player.PCOL);

	do
	{
		SETSCL();
		a = dungeon.MAZLND[dungeon.RC2IDX(dungeon.DROW.row, dungeon.DROW.col)];
		u = 0;
		x = 4;
		do
		{
			b = a;
			b = (b & 3);
			dungeon.NEIBOR[u+4] = b;
			dungeon.NEIBOR[u] = b;
			++u;
			a >>= 2;
			--x;
		} while (x != 0);

		b = player.PDIR;
		u = b;

		for (ftctr = 0; ftctr < 3; ++ftctr)
		{
			b = dungeon.NEIBOR[u + FLATAB[ftctr]];
			if (b == dungeon.HF_SDR)
			{
				--MAGFLG;
				DRAWIT(FLATABv[ftctr][b]);
				b = dungeon.HF_WAL;
			}
			DRAWIT(FLATABv[ftctr][b]);
		}

		creNum = creature.CFIND2(dungeon.DROW);
		if (creNum != -1)
		{
			CMRDRW(FWDCRE[creature.CCBLND[creNum].creature_id], creNum);
		}

		PDRAW(LPK_VLA, 3, u);
		PDRAW(RPK_VLA, 1, u);

		// Draw vertical features
		vft = dungeon.VFIND(dungeon.DROW);
		if (vft == Dungeon::VF_NULL)
		{
			DRAWIT(CEI_VLA);
		}
		else
		{
			switch(vft)
			{
			case Dungeon::VF_HOLE_UP:
				DRAWIT(HUP_VLA);
				break;
			case Dungeon::VF_LADDER_UP:
				DRAWIT(LAD_VLA);
				DRAWIT(HUP_VLA);
				break;
			case Dungeon::VF_HOLE_DOWN:
				DRAWIT(HDN_VLA);
				DRAWIT(CEI_VLA);
				break;
			case Dungeon::VF_LADDER_DOWN:
				DRAWIT(LAD_VLA);
				DRAWIT(HDN_VLA);
				DRAWIT(CEI_VLA);
				break;
			default:
				break; // should never get here
			}
		}

		// Draw Objects
		object.OFINDF = 0;
		do
		{
			objIdx = object.OFIND(dungeon.DROW);
			if (objIdx == -1)
				break;
			--MAGFLG;
			DRAWIT(FWDOBJ[object.OCBLND[objIdx].obj_type]);
			DRAWIT(FWDOBJ[object.OCBLND[objIdx].obj_type]);
		} while (true);


		if (dungeon.NEIBOR[u] != 0)
		{
			break;
		}
		dungeon.DROW.row += dungeon.STPTAB[player.PDIR * 2];
		dungeon.DROW.col += dungeon.STPTAB[player.PDIR * 2 + 1];
		++RANGE;
	} while (RANGE <= 9);
}

// Calculates fade (line-pixelation) based on lighting
void Viewer::SETFAD()
{
	dodBYTE a, b;
	a = RLIGHT;
	if (MAGFLG != 0)
	{
		a = MLIGHT;
		MAGFLG = 0;
	}
	b = 0;
	a -= 7;
	a -= RANGE;
	if ((a & 128) != 0)
	{
		--b;
		if ((((a & 128) != 0) && a > 0xF9) || ((a & 128) == 0)) //	if (a > -7)
		{
			b = (1 << (-1 - a));
		}
	}
	VCTFAD = b;
}

// Used by 3D-Viewer, draws a vector list
void Viewer::DRAWIT(int * vl)
{
	SETFAD();
	drawVectorList(vl);
}

// Used by 3D-Viewer, checks for around-the-corner creature
void Viewer:: PDRAW(int * vl, dodBYTE dir, dodBYTE pdir)
{
	RowCol side;
	dodBYTE DIR;
	int creNum;
	if (dungeon.NEIBOR[pdir + dir] != 0)
		return;
	DIR = ((dir + player.PDIR) & 3);
	side.row = dungeon.DROW.row + dungeon.STPTAB[DIR * 2];
	side.col = dungeon.DROW.col + dungeon.STPTAB[DIR * 2 + 1];
	creNum = creature.CFIND2(side);
	if (creNum == -1)
		return;
	CMRDRW(vl, creNum);
}

// Prepares for drawing creature with either magical or physical lighting
void Viewer::CMRDRW(int * vl, int creNum)
{
	if (creature.CCBLND[creNum].P_CCMGO != 0)
		--MAGFLG;
	DRAWIT(vl);
}

// Sets the perspective scale
void Viewer::SETSCL()
{
	int idx = HLFSCL;
	if (HLFSTP == 0)
	{
		++idx;
		if (BAKSTP == 0)
		{
			idx = 0;
		}
	}
	VXSCAL = Scale[idx + RANGE];
	VYSCAL = Scale[idx + RANGE];
	VXSCALf = Scalef[idx + RANGE];
	VYSCALf = Scalef[idx + RANGE];
}

void Viewer::EXAMIN()
{
	TXB_U = &TXTEXA;
	--TXBFLG;
	NEWLIN = 0;
	TXB_U->caret = 10;
	OUTSTI(exam1);

	// check for creature
	if (creature.CFIND2(RowCol(player.PROW, player.PCOL)) != -1)
	{
		TXB_U->caret += 11;
		OUTSTI(exam2);
	}

	// check for objects on floor
	int idx;
	object.OFINDF = 0;
	do
	{
		idx = object.OFIND(RowCol(player.PROW, player.PCOL));
		if (idx != -1)
		{
			PRTOBJ(idx, false);
		}
	} while (idx != -1);

	if (NEWLIN != 0)
	{
		PCRLF();
	}

	int ctr = 32;
	do
	{
		OUTCHR(parser.I_EXCL);
		--ctr;
	} while (ctr != 0);

	TXB_U->caret += 12;
	OUTSTI(exam3);

	// bag contents
	ctr = player.BAGPTR;
	while (ctr != -1)
	{
		// check for torch
		if (ctr == player.PTORCH)
		{
			PRTOBJ(ctr, true);
		}
		else
		{
			PRTOBJ(ctr, false);
		}
		ctr = object.OCBLND[ctr].P_OCPTR;
	}

	TXBFLG = 0;
}

void Viewer::PCRLF()
{
	OUTCHR(parser.I_CR);
	NEWLIN = 0;
}

void Viewer::PRTOBJ(int X, bool highlite)
{
	object.OBJNAM(X);
	if (highlite)
	{
		tcaret = TXB_U->caret;
		int ctr = 0;
		while (parser.TOKEN[ctr] != 0xFF)
		{
			++ctr;
		}
		tlen = ctr;
	}
	OUTSTR(parser.TOKEN);
	NEWLIN = (~NEWLIN);
	if (NEWLIN != 0)
	{
		TXB_U->caret += 16;
		TXB_U->caret = (TXB_U->caret / 16) * 16;
	}
	else
	{
		PCRLF();
	}
}

// Draws the map; showSeerMap bool determines VISION or SEER mode
void Viewer::MAPPER()
{
	int mazIdx, objIdx, creIdx, vftIdx;
	float DoorOffset;
	RowCol rc;
	dodBYTE a;
	bool vftOnce;

	dungeon.DROW.row = 31;
	dungeon.DROW.col = 31;
	_renderer->setColor(0.0,0.0,0.0);
	do
	{
		mazIdx = dungeon.RC2IDX(dungeon.DROW.row, dungeon.DROW.col);
		if (dungeon.MAZLND[mazIdx] != 0xFF)
		{
			_renderer->drawQuad(
				crd.newX(dungeon.DROW.col * 8), crd.newY(dungeon.DROW.row * 6),
				crd.newX(dungeon.DROW.col * 8), crd.newY((dungeon.DROW.row + 1) * 6),
				crd.newX((dungeon.DROW.col + 1) * 8), crd.newY((dungeon.DROW.row + 1) * 6),
				crd.newX((dungeon.DROW.col + 1) * 8), crd.newY(dungeon.DROW.row * 6));

			if (game.MarkDoorsOnScrollMaps) {  //Do we need to mark the doors on the scroll maps?
				if ((dungeon.MAZLND[mazIdx] & 0x0c) == (0x01 << 2) ||
					(dungeon.MAZLND[mazIdx] & 0x0c) == (0x02 << 2))
				{  //Do we have a east door or secret door?
					DoorOffset = ((dungeon.MAZLND[mazIdx] | 0xcc) != 0xff);  //Move door line over one into next room if we don't have wall on either side.
					DoorOffset = DoorOffset / 4;
					_renderer->setColor(1.0,1.0,1.0);
					if ((dungeon.MAZLND[mazIdx] & 0x0c) == (0x01 << 2)) {  //Is this a regular door?  Yes:
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6),
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.5));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4),
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY((dungeon.DROW.row + 1) * 6));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.5),
							crd.newX((dungeon.DROW.col + 1) * 8 - 1 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.5));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.5),
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4),
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.5));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 - 1 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4),
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4));
						
					} else {  //Is this a regular door?  No:
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6),
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 1.75));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4.5),
							crd.newX((dungeon.DROW.col + 1) * 8 + DoorOffset), crd.newY((dungeon.DROW.row + 1) * 6));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.25),
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.25));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 2.25),
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 3));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.5 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 3.25),
							crd.newX((dungeon.DROW.col + 1) * 8 - 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 3.25));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 3.25),
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.75 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4));
						_renderer->drawLine(
							crd.newX((dungeon.DROW.col + 1) * 8 - 1 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4.25),
							crd.newX((dungeon.DROW.col + 1) * 8 + 0.5 + DoorOffset), crd.newY(dungeon.DROW.row * 6 + 4.25));
					}  //Is this a regular door?
					_renderer->setColor(0.0,0.0,0.0);
				}  //Do we have a east door or secret door?
				if ((dungeon.MAZLND[mazIdx] & 0x30) == (0x01 << 4) ||
					(dungeon.MAZLND[mazIdx] & 0x30) == (0x02 << 4))
				{  //Do we have a south door or secret door?
					DoorOffset = ((dungeon.MAZLND[mazIdx] | 0x33) != 0xff);  //Move door line over one into next room if we don't have wall on either side.
					DoorOffset = DoorOffset / 4;
					_renderer->setColor(1.0,1.0,1.0);
					if ((dungeon.MAZLND[mazIdx] & 0x30) == (0x01 << 4)) {  //Is this a regular door?  Yes:
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX((dungeon.DROW.col + 1) * 8), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3), crd.newY((dungeon.DROW.row + 1) * 6 - 0.75 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 - 0.75 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 - 0.75 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 + 0.75 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 + 0.75 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 - 0.75 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 + 0.75 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 + 0.75 + DoorOffset));
					} else {  //Is this a regular door?  No:
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 2.75), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 5), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX((dungeon.DROW.col + 1) * 8), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 - 1 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 - 1 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 - 1 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 - 0.25 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3.25), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.5), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.75), crd.newY((dungeon.DROW.row + 1) * 6 + 0.75 + DoorOffset));
						_renderer->drawLine(
							crd.newX(dungeon.DROW.col * 8 + 3), crd.newY((dungeon.DROW.row + 1) * 6 + 1 + DoorOffset),
							crd.newX(dungeon.DROW.col * 8 + 4.5), crd.newY((dungeon.DROW.row + 1) * 6 + 1 + DoorOffset));
					}
					_renderer->setColor(0.0,0.0,0.0);
				}  //Do we have a south door or secret door?
			}  //Do we need to mark the doors on the scroll maps?
		}
		--dungeon.DROW.col;
		if (dungeon.DROW.col == 0xFF)
		{
			--dungeon.DROW.row;
			dungeon.DROW.col = 31;
		}
	} while (dungeon.DROW.row != 0xFF);

	_renderer->setColor(1.0,1.0,1.0);
	if (showSeerMap == true)
	{
		// Mark Objects
		object.OFINDF = 0;
		do
		{
			objIdx = object.FNDOBJ();
			if (objIdx == -1)
				break;
			if (object.OCBLND[objIdx].P_OCOWN != 0)
				continue;
			rc.row = object.OCBLND[objIdx].P_OCROW;
			rc.col = object.OCBLND[objIdx].P_OCCOL;
			_renderer->drawQuad(
				crd.newX((rc.col * 8) + 4), crd.newY((rc.row * 6) + 2),
				crd.newX((rc.col * 8) + 4), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 2));
		} while (true);

		// Mark Creatures
		creIdx = -1;
		do
		{
			++creIdx;
			if (creIdx == 32)
				break;
			if (creature.CCBLND[creIdx].P_CCUSE == 0)
				continue;
			rc.row = creature.CCBLND[creIdx].P_CCROW;
			rc.col = creature.CCBLND[creIdx].P_CCCOL;
			_renderer->drawQuad(
				crd.newX((rc.col * 8) + 1), crd.newY((rc.row * 6) + 2),
				crd.newX((rc.col * 8) + 1), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 2));

			_renderer->drawQuad(
				crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 2),
				crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 4),
				crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 2));

			_renderer->drawQuad(
				crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 1),
				crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 5),
				crd.newX((rc.col * 8) + 4), crd.newY((rc.row * 6) + 5),
				crd.newX((rc.col * 8) + 4), crd.newY((rc.row * 6) + 1));
		} while (true);
	}

	// Mark Player
	rc.row = player.PROW;
	rc.col = player.PCOL;
	_renderer->drawQuad(
		crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 1),
		crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 2),
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 2),
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 1));

	_renderer->drawQuad(
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 1),
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 2),
		crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 2),
		crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 1));

	_renderer->drawQuad(
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 2),
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 4),
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 4),
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 2));

	_renderer->drawQuad(
		crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 4),
		crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 5),
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 5),
		crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 4));

	_renderer->drawQuad(
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 4),
		crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 5),
		crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 5),
		crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 4));

	// Mark Vertical Features
	vftIdx = dungeon.VFTPTR;
	vftOnce = false;
	do
	{
		a = dungeon.VFTTAB[vftIdx++];
		if (a == 0xFF)
		{
			if (vftOnce == false)
			{
				vftOnce = true;
				continue;
			}
			else
			{
				break;
			}
		}
		rc.row = dungeon.VFTTAB[vftIdx++];
		rc.col = dungeon.VFTTAB[vftIdx++];

		_renderer->drawQuad(
			crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 1),
			crd.newX((rc.col * 8) + 2), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 1));

		_renderer->drawQuad(
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 1),
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 6), crd.newY((rc.row * 6) + 1));

		_renderer->drawQuad(
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 1),
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 2),
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 2),
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 1));

		_renderer->drawQuad(
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 4),
			crd.newX((rc.col * 8) + 3), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 5),
			crd.newX((rc.col * 8) + 5), crd.newY((rc.row * 6) + 4));

	} while (true);
}

// Draws non-font vector lists
void Viewer::drawVectorList(int VLA[])
{
	int numLists = VLA[0];
	int curList = 0;
	int numVertices;
	int curVertex;
	int	ctr = 1;

	if (VCTFAD == 0xFF)
	{
		return;
	}

	while (curList < numLists)
	{
		numVertices = VLA[ctr];
		++ctr;
		curVertex = 0;
		while (curVertex < (numVertices - 1) )
		{
			if (g_options&(OPT_VECTOR|OPT_HIRES)) {
				float x0, y0, x1, y1;
				x0 = ScaleXf((float)VLA[ctr]) + (float)VCNTRX;
				y0 = ScaleYf((float)VLA[ctr+1]) + (float)VCNTRY;
				x1 = ScaleXf((float)VLA[ctr+2]) + (float)VCNTRX;
				y1 = ScaleYf((float)VLA[ctr+3]) + (float)VCNTRY;
				_renderer->drawVector(x0, y0, x1, y1);
			}
			else {
				float x0, y0, x1, y1;
				x0 = ScaleXf((float)VLA[ctr]) + (float)VCNTRX;
				y0 = ScaleYf((float)VLA[ctr+1]) + (float)VCNTRY;
				x1 = ScaleXf((float)VLA[ctr+2]) + (float)VCNTRX;
				y1 = ScaleYf((float)VLA[ctr+3]) + (float)VCNTRY;

				_renderer->drawVector((float)(int)x0, (float)(int)y0, (float)(int)x1, (float)(int)y1);

//				dodSHORT x0, y0, x1, y1;
//				x0 = ScaleX(VLA[ctr]) + VCNTRX;
//				y0 = ScaleY(VLA[ctr+1]) + VCNTRY;
//				x1 = ScaleX(VLA[ctr+2]) + VCNTRX;
//				y1 = ScaleY(VLA[ctr+3]) + VCNTRY;
//				drawVector(x0, y0, x1, y1);
			}
			ctr += 2;
			++curVertex;
		}
		++curList;
		ctr += 2;
	}
}

// Scales X-coordinate
dodSHORT Viewer::ScaleX(int x)
{
	return ((x-VCNTRX)*VXSCAL)/127;
}

float Viewer::ScaleXf(float x)
{
	return ((x-(float)VCNTRX)*VXSCALf)/127.0f;
}

float Viewer::ScaleYf(float y)
{
	return ((y-VCNTRY)*VYSCALf)/127.0f;
}

// Scales Y-coordinate
dodSHORT Viewer::ScaleY(int y)
{
	return ((y-VCNTRY)*VYSCAL)/127;

}

// Draws font vectors
void Viewer::drawVectorListAQ(int VLA[])
{
	int numQuads = VLA[0];
	int curQuad = 0;
	int	ctr = 1;

	while (curQuad < numQuads)
	{
		_renderer->drawQuad(
			crd.newXa((double) VLA[ctr]),crd.newYa((double) VLA[ctr+1]),
			crd.newXa((double) VLA[ctr+2]),crd.newYa((double) VLA[ctr+3]),
			crd.newXa((double) VLA[ctr+4]),crd.newYa((double) VLA[ctr+5]),
			crd.newXa((double) VLA[ctr+6]),crd.newYa((double) VLA[ctr+7]));
		ctr += 8;
		++curQuad;
	}
}

// Draws a character
void Viewer::drawCharacter(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		drawVectorListAQ(AZ_VLA[c-64]);
	}
	if (c >= '0' && c <= '9')
	{
		drawVectorListAQ(AZ_VLA[c-13]); //Dependent on ASCII values
	}
	switch (c)
	{
	case ('<'):
		drawVectorListAQ(AZ_VLA[31]);
		break;
	case ('>'):
		drawVectorListAQ(AZ_VLA[32]);
		break;
	case ('{'):
		drawVectorListAQ(AZ_VLA[33]);
		break;
	case ('}'):
		drawVectorListAQ(AZ_VLA[34]);
		break;
	case (' '):
		break;
	case ('.'):
		drawVectorListAQ(AZ_VLA[27]);
		break;
	case ('_'):
		drawVectorListAQ(AZ_VLA[28]);
		break;
	case ('!'):
		drawVectorListAQ(AZ_VLA[29]);
		break;
	case ('?'):
		drawVectorListAQ(AZ_VLA[30]);
		break;
	case ('/'):
		drawVectorListAQ(AZ_VLA[45]);
		break;
	case ('\\'):
		drawVectorListAQ(AZ_VLA[46]);
		break;
	case ('%'):
		drawVectorListAQ(AZ_VLA[47]);
		break;
	case ('+'):
		drawVectorListAQ(AZ_VLA[48]);
		break;
	case ('-'):
		drawVectorListAQ(AZ_VLA[49]);
		break;
	default:
		break;
	}
}

void Viewer::drawString_internal(int x, int y, dodBYTE * str, int len)
{
	int ctr;
	char c;
	_renderer->resetMatrix();
	_renderer->setTranslation(crd.newX(x*8), crd.newY((y+1)*8));
	for (ctr=0; ctr < len; ++ctr)
	{
		c = dod_to_ascii(*(str + ctr));
		drawCharacter(c);
		_renderer->setTranslation(crd.newXa(8), 0.0);
	}
}

char Viewer::dod_to_ascii(dodBYTE c)
{
	if (c >= 1 && c <= 26) return (c | 64);
	if (c == parser.I_SP) return ' ';
	if (c == parser.I_EXCL) return '!';
	if (c == parser.I_BAR) return '_';
	if (c == parser.I_QUES) return '?';
	if (c == parser.I_DOT) return '.';
	if (c == parser.I_SHL) return '<';
	if (c == parser.I_SHR) return '>';
	if (c == parser.I_LHL) return '{';
	if (c == parser.I_LHR) return '}';
	return ' ';
}

// Draws a string
void Viewer::drawString(int x, int y, const char * str, int len)
{
	int ctr;
	_renderer->resetMatrix();
	_renderer->setTranslation(crd.newX(x*8), crd.newY((y+1)*8));
	for (ctr=0; ctr < len; ++ctr)
	{
		drawCharacter(*(str + ctr));
		_renderer->setTranslation(crd.newXa(8), 0.0);
	}
}


/*void Viewer::drawKeyboard(struct kbdKey key)
{
	int squareWidth = key.width;
	int squareHeight = key.height;
	int highlightX = key.x;
	int highlightY = key.y;

	glEnable(GL_TEXTURE_2D);
	// Clear screen
	_renderer->setColor(bgColor);
	_renderer->clearBuffer(true);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  	glBindTexture(GL_TEXTURE_2D, keyboardTexture);
  	_renderer->resetMatrix();

 	glBegin(GL_QUADS);
    	glTexCoord2f(0,1);
    	glVertex2f(0, 0);

    	glTexCoord2f(1,1);
    	glVertex2f(480, 0);

    	glTexCoord2f(1,0);
    	glVertex2f(480, 272);

    	glTexCoord2f(0,0);
    	glVertex2f(0, 272);

  	glEnd();
  	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINE_STRIP);
		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX + squareWidth, highlightY);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX + squareWidth, highlightY + squareHeight);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY + squareHeight);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY);
	glEnd();

 	// Draw Boxes for menu
 	_renderer->setColor(fgColor);
 	_renderer->resetMatrix();

  	drawString(0, 0, oslink.commandCreatorBuffer, strlen(oslink.commandCreatorBuffer));

  	_renderer->swapBuffers();

}*/

/****************************************************************
  Member: drawCommandMenu

  Vars:   menu_id   - the menu number to draw
          highlight - the menu item to highlight

  Function: Draws the command creator menu
****************************************************************/
void Viewer::drawCommandMenu(command_menu commandMenu, int menu_id, int highlight)
 {
 int x, y, length;

 // Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);

  // Draw Boxes for menu
 _renderer->setColor(fgColor);
 _renderer->resetMatrix();

  // Draw Menu Items
  drawString(0, 0, oslink.commandCreatorBuffer, strlen(oslink.commandCreatorBuffer));

  drawString(0/*menu_id * 5*/, 2, commandMenu.getMenuName(menu_id),
                             strlen(commandMenu.getMenuName(menu_id)));


 for(int i = 0; i < commandMenu.getMenuSize(menu_id); i++)
   {
   x = 0;//menu_id * 5;
   y = i + 4;
   length = strlen(commandMenu.getMenuItem(menu_id, i));

   if(i == highlight)
     {
     _renderer->setColor(fgColor);
     _renderer->resetMatrix();
     _renderer->drawQuad(
     	crd.newX(x * 8), crd.newY(y * 8),
    	crd.newX((x + length) * 8), crd.newY(y * 8),
     	crd.newX((x + length) * 8), crd.newY((y + 1) * 8),
     	crd.newX(x * 8), crd.newY((y + 1) * 8));
     _renderer->setColor(bgColor);
     }

   drawString(x, y, commandMenu.getMenuItem(menu_id, i), length);
   _renderer->setColor(fgColor);
   }

  // Update the screen
 _renderer->swapBuffers();
 }


/****************************************************************
  Member: drawMenu

  Vars:   menu_id   - the menu number to draw
          highlight - the menu item to highlight

  Function: Draws the menu
****************************************************************/
void Viewer::drawMenu(menu mainMenu, int menu_id, int highlight)
 {
 int x, y, length;

 // Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);

  // Draw Boxes for menu
 _renderer->setColor(fgColor);
 _renderer->resetMatrix();

  // Draw Menu Items
 drawString(menu_id * 5, 0, mainMenu.getMenuName(menu_id),
                             strlen(mainMenu.getMenuName(menu_id)));

 for(int i = 0; i < mainMenu.getMenuSize(menu_id); i++)
   {
   x = menu_id * 5;
   y = i + 2;
   length = strlen(mainMenu.getMenuItem(menu_id, i));

   if(i == highlight)
     {
     _renderer->setColor(fgColor);
     _renderer->resetMatrix();
     _renderer->drawQuad(
     	crd.newX(x * 8), crd.newY(y * 8),
     	crd.newX((x + length) * 8), crd.newY(y * 8),
     	crd.newX((x + length) * 8), crd.newY((y + 1) * 8),
     	crd.newX(x * 8), crd.newY((y + 1) * 8));
     _renderer->setColor(bgColor);
     }

   drawString(x, y, mainMenu.getMenuItem(menu_id, i), length);
   _renderer->setColor(fgColor);
   }

  // Update the screen
 _renderer->swapBuffers();
 }

/****************************************************************
  Member: drawMenuList

  Vars:   x         - the top-left x-coordinate
          y         - the top-left y-coordinate
	  title     - the title of the list
	  list      - the list to be drawn
	  listSize  - the size of the list
	  highlight - the item that's highlighted

  Function: Draws a menu list
****************************************************************/
void Viewer::drawMenuList(int x, int y, char *title, char *list[], int listSize, int highlight)
 {
 int length;

  // Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);
 _renderer->setColor(fgColor);

 drawString(x, y, title, strlen(title));
 y += 2;

  // Draw Menu Items
 for(int i = 0; i < listSize; i++, y++)
   {
   length = strlen(list[i]);

   if(i == highlight)
     {
     _renderer->setColor(fgColor);
     _renderer->resetMatrix();
     _renderer->drawQuad(
     	crd.newX(x * 8), crd.newY(y * 8),
     	crd.newX((x + length) * 8), crd.newY(y * 8),
     	crd.newX((x + length) * 8), crd.newY((y + 1) * 8),
     	crd.newX(x * 8), crd.newY((y + 1) * 8));
     _renderer->setColor(bgColor);
     }

   drawString(x, y, list[i], length);
   _renderer->setColor(fgColor);
   }

  // Update the screen
 _renderer->swapBuffers();
 }


/****************************************************************
  Member: drawMenuScrollbar

  Vars:   title   - the title of the scrollbar
          current - the current percentage

  Function: Draws a menu scroll bar
****************************************************************/
void Viewer::drawMenuScrollbar(char *title, int current)
 {
 int x;

	// Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);
 _renderer->setColor(fgColor);

 drawString(0, 0, title, strlen(title));
 drawString(0, 2, "USE ARROW KEYS TO NAVIGATE", 26);
 drawString(0, 3, "PRESS ENTER WHEN FINISHED", 25);
 drawString(0, 4, "OR ESCAPE TO CANCEL", 19);
 drawString(0, 6, "MIN", 3);
 drawString(29,6, "MAX", 3);

 for(x = 0; x < current; x++)
  {
  drawString(x, 7, "-", 1);
  }

 drawString(current, 7, "+", 1);

 for(x = current + 1; x < 32; x++)
  {
  drawString(x, 7, "-", 1);
  }

  // Update the screen
 _renderer->swapBuffers();
 }

/****************************************************************
  Member: drawMenuStringTitle

  Vars:   title - the title of the string box

  Function: Draws a menu string box
****************************************************************/
void Viewer::drawMenuStringTitle(char *title)
 {
  // Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);
 _renderer->setColor(fgColor);

 drawString(0, 0, title, strlen(title));

  // Update the screen
 _renderer->swapBuffers();
 }

/****************************************************************
  Member: drawMenuString

  Vars:   title - the title of the string box

  Function: Draws a menu string box
****************************************************************/
void Viewer::drawMenuString(char *currentString)
 {
 drawString(0, 2, currentString, strlen(currentString));
 drawString(strlen(currentString), 2, "_", 1);

  // Update the screen
 _renderer->swapBuffers();
 }

/****************************************************************
  Member: aboutBox

  Function: Draws the "About" Box
****************************************************************/
void Viewer::aboutBox(void)
 {
  // Clear screen
 _renderer->setColor(bgColor);
 _renderer->clearBuffer(true);
 _renderer->setColor(fgColor);

 int i = 3;

 drawString(0, i++, "ABOUT DUNGEONS OF DAGGORATH", 27);
 drawString(0, i++, "COPYRIGHT 1982 DYNAMICRO", 24);
 drawString(0, i++, "ORIGINAL DESIGN. DOUGLAS MORGAN", 31);
 drawString(0, i++, "PC PORT BY RICHARD HUNERLACH", 28);
 drawString(0, i++, "PSP PORT BY TREVOR GERBRAND", 27);
 drawString(0, i++, "VARIOUS OTHER CONTRIBUTIONS BY", 30);
 drawString(1, i++, "TIM LINDNER", 11);
 drawString(1, i++, "AARON OLIVER", 12);
 drawString(1, i++, "ANTHONY BUTTIGIEG", 17);
 drawString(1, i++, "BRUCE", 5);
 drawString(1, i++, "JOE LAVIGNE", 11);
 drawString(1, i++, "KEN THOMPSON", 12);
 drawString(1, i++, "ERICK RITCHIE", 13);
 drawString(1, i++, "MATTHEW EYSTER", 14);
 drawString(1, i++, "DAN GENDREAU", 12);
 drawString(1, i++, "JOSH ALBRIGHT", 13);
 drawString(1, i++, "MANY OTHERS NOT MENTIONED HERE!", 31);

  // Update the screen
 _renderer->swapBuffers();
 }
