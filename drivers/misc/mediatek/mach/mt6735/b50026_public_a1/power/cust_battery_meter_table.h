#ifndef _CUST_BATTERY_METER_TABLE_H
#define _CUST_BATTERY_METER_TABLE_H

#include <mt-plat/battery_meter.h>
#define BAT_NTC_10 1
#define BAT_NTC_47 0

#if (BAT_NTC_10 == 1)
#define RBAT_PULL_UP_R             16900
#endif

#if (BAT_NTC_47 == 1)
#define RBAT_PULL_UP_R             61900
#endif

#define RBAT_PULL_UP_VOLT          1800

typedef struct _BATTERY_PROFILE_STRUCT {
	signed int percentage;
	signed int voltage;
} BATTERY_PROFILE_STRUCT, *BATTERY_PROFILE_STRUCT_P;

typedef struct _R_PROFILE_STRUCT {
	signed int resistance;
	signed int voltage;
} R_PROFILE_STRUCT, *R_PROFILE_STRUCT_P;

typedef enum {
	T1_0C,
	T2_25C,
	T3_50C
} PROFILE_TEMPERATURE;

#if (BAT_NTC_10 == 1)
BATT_TEMPERATURE Batt_Temperature_Table[] = {
	{-20, 68237},
	{-15, 53650},
	{-10, 42506},
	{ -5, 33892},
	{  0, 27219},
	{  5, 22021},
	{ 10, 17926},
	{ 15, 14674},
	{ 20, 12081},
	{ 25, 10000},
	{ 30, 8315},
	{ 35, 6948},
	{ 40, 5834},
	{ 45, 4917},
	{ 50, 4161},
	{ 55, 3535},
	{ 60, 3014}
};
#endif

#if (BAT_NTC_47 == 1)
	BATT_TEMPERATURE Batt_Temperature_Table[] = {
	{-20, 483954},
	{-15, 360850},
	{-10, 271697},
	{ -5, 206463},
	{  0, 158214},
	{  5, 122259},
	{ 10, 95227},
	{ 15, 74730},
	{ 20, 59065},
	{ 25, 47000},
	{ 30, 37643},
	{ 35, 30334},
	{ 40, 24591},
	{ 45, 20048},
	{ 50, 16433},
	{ 55, 13539},
	{ 60, 11210}
	};
#endif

// T0 -10C
BATTERY_PROFILE_STRUCT battery_profile_t0[] =
{
{0,4351},	 
{1,4308},	 
{2,4279},	 
{3,4254},	 
{4,4233},	 
{5,4212},	 
{6,4194},	 
{7,4176},	 
{8,4159},	 
{9,4143},	 
{10,4128},	
{11,4113},	
{12,4100},	
{13,4086},	
{14,4074},	
{15,4061},	
{17,4049},	
{18,4036},	
{19,4024},	
{20,4012},	
{21,4000},	
{22,3989},	
{23,3979},	
{24,3969},	
{25,3960},	
{26,3951},	
{27,3942},	
{28,3933},	
{29,3925},	
{30,3917},	
{31,3909},	
{32,3901},	
{34,3893},	
{35,3886},	
{36,3878},	
{37,3871},	
{38,3865},	
{39,3858},	
{40,3852},	
{41,3846},	
{42,3840},	
{43,3834},	
{44,3828},	
{45,3823},	
{46,3818},	
{47,3813},	
{48,3808},	
{50,3804},	
{51,3800},	
{52,3796},	
{53,3793},	
{54,3789},	
{55,3786},	
{56,3783},	
{57,3780},	
{58,3777},	
{59,3774},	
{60,3771},	
{61,3768},	
{62,3765},	
{63,3763},	
{64,3760},	
{65,3757},	
{67,3754},	
{68,3751},	
{69,3749},	
{70,3745},	
{71,3743},	
{72,3740},	
{73,3737},	
{74,3734},	
{75,3731},	
{76,3728},	
{77,3725},	
{78,3722},	
{79,3718},	
{80,3715},	
{81,3711},	
{83,3707},	
{84,3703},	
{85,3699},	
{86,3695},	
{87,3690},	
{88,3685},	
{89,3679},	
{90,3674},	
{91,3654},	
{92,3618},	
{93,3587},	
{94,3564},	
{95,3542},	
{96,3507},	
{97,3476},	
{98,3438},	
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
};      
        
// T1 0C 
BATTERY_PROFILE_STRUCT battery_profile_t1[] =
{
{0,4347},	 
{1,4311},	 
{2,4290},	 
{3,4272},	 
{4,4257},	 
{5,4242},	 
{6,4228},	 
{7,4214},	 
{8,4201},	 
{9,4188},	 
{10,4175},	
{11,4162},	
{12,4150},	
{13,4138},	
{14,4126},	
{15,4114},	
{16,4102},	
{17,4090},	
{18,4079},	
{19,4067},	
{20,4057},	
{21,4046},	
{22,4034},	
{23,4023},	
{24,4012},	
{25,4000},	
{26,3990},	
{27,3979},	
{28,3969},	
{29,3960},	
{30,3951},	
{31,3942},	
{32,3933},	
{33,3924},	
{34,3916},	
{35,3908},	
{36,3900},	
{37,3893},	
{38,3885},	
{39,3878},	
{40,3871},	
{41,3864},	
{42,3857},	
{43,3851},	
{44,3844},	
{45,3838},	
{47,3833},	
{48,3827},	
{49,3821},	
{50,3816},	
{51,3811},	
{52,3806},	
{53,3802},	
{54,3797},	
{55,3793},	
{56,3789},	
{57,3786},	
{58,3782},	
{59,3780},	
{60,3777},	
{61,3774},	
{62,3772},	
{63,3769},	
{64,3767},	
{65,3764},	
{66,3762},	
{67,3759},	
{68,3757},	
{69,3754},	
{70,3751},	
{71,3749},	
{72,3746},	
{73,3743},	
{74,3740},	
{75,3737},	
{76,3734},	
{77,3731},	
{78,3727},	
{79,3724},	
{80,3721},	
{81,3717},	
{82,3714},	
{83,3710},	
{84,3706},	
{85,3702},	
{86,3699},	
{87,3695},	
{88,3691},	
{89,3686},	
{90,3681},	
{91,3676},	
{93,3663},	
{94,3596},	
{95,3550},	
{96,3516},	
{97,3495},	
{98,3464},	
{99,3443},	
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
{100,3400},
};           

// T2 25C
BATTERY_PROFILE_STRUCT battery_profile_t2[] =
{
{0,4350},	
{0,4334},	
{1,4318},	
{2,4304},	
{3,4292},	
{4,4279},	
{5,4267},	
{6,4255},	
{7,4244},	
{8,4233},	
{9,4221},	
{10,4209},	
{11,4198},	
{12,4187},	
{13,4176},	
{14,4165},	
{15,4154},	
{16,4143},	
{17,4132},	
{18,4122},	
{19,4111},	
{20,4101},	
{21,4090},	
{22,4080},	
{23,4069},	
{24,4060},	
{25,4050},	
{26,4040},	
{26,4030},	
{27,4021},	
{28,4011},	
{29,4002},	
{30,3992},	
{31,3983},	
{32,3974},	
{33,3965},	
{34,3956},	
{35,3947},	
{36,3938},	
{37,3929},	
{38,3920},	
{39,3911},	
{40,3902},	
{41,3894},	
{42,3886},	
{43,3878},	
{44,3871},	
{45,3864},	
{46,3857},	
{47,3851},	
{48,3845},	
{49,3840},	
{50,3834},	
{51,3829},	
{52,3825},	
{53,3820},	
{53,3816},	
{54,3811},	
{55,3807},	
{56,3803},	
{57,3799},	
{58,3796},	
{59,3792},	
{60,3789},	
{61,3786},	
{62,3782},	
{63,3779},	
{64,3776},	
{65,3774},	
{66,3771},	
{67,3768},	
{68,3766},	
{69,3763},	
{70,3761},	
{71,3759},	
{72,3757},	
{73,3755},	
{74,3752},	
{75,3749},	
{76,3746},	
{77,3743},	
{78,3740},	
{79,3736},	
{80,3733},	
{80,3729},	
{81,3725},	
{82,3721},	
{83,3715},	
{84,3710},	
{85,3704},	
{86,3698},	
{87,3691},	
{88,3683},	
{89,3676},	
{90,3671},	
{91,3668},	
{92,3664},	
{93,3659},	
{94,3652},	
{95,3642},	
{96,3622},	
{97,3586},	
{98,3536},	
{99,3468},	
{100,3400},
};     

// T3 50C
BATTERY_PROFILE_STRUCT battery_profile_t3[] =
{
{0,4348},	 
{0,4314},	 
{1,4301},	 
{2,4288},	 
{3,4276},	 
{4,4264},	 
{5,4252},	 
{6,4241},	 
{7,4228},	 
{8,4217},	 
{9,4206},	 
{10,4195}, 
{11,4184},	
{12,4173},	
{13,4162},	
{14,4151},	
{15,4140},	
{16,4130},	
{17,4119},	
{18,4108},	
{19,4098},	
{20,4088},	
{21,4078},	
{22,4068},	
{23,4058},	
{24,4048},	
{25,4039},	
{26,4029},	
{27,4020},	
{28,4011},	
{29,4002},	
{30,3994},	
{31,3985},	
{32,3977},	
{33,3969},	
{34,3960},	
{35,3953},	
{36,3944},	
{37,3936},	
{38,3928},	
{39,3920},	
{40,3912},	
{41,3902},	
{41,3893},	
{42,3884},	
{43,3874},	
{44,3866},	
{45,3858},	
{46,3851},	
{47,3845},	
{48,3840},	
{49,3834},	
{50,3829},	
{51,3824},	
{52,3820},	
{53,3815},	
{54,3811},	
{55,3807},	
{56,3803},	
{57,3799},	
{58,3795},	
{59,3792},	
{60,3789},	
{61,3785},	
{62,3782},	
{63,3779},	
{64,3776},	
{65,3773},	
{66,3771},	
{67,3768},	
{68,3766},	
{69,3763},	
{70,3760},	
{71,3756},	
{72,3752},	
{73,3746},	
{74,3741},	
{75,3738},	
{76,3734},	
{77,3730},	
{78,3726},	
{79,3722},	
{80,3718},	
{81,3715},	
{82,3711},	
{82,3707},	
{83,3702},	
{84,3696},	
{85,3690},	
{86,3685},	
{87,3679},	
{88,3670},	
{89,3664},	
{90,3662},	
{91,3660},	
{92,3657},	
{93,3653},	
{94,3647},	
{95,3636},	
{96,3610},	
{97,3568},	
{98,3514},	
{99,3443},	
{100,3400},
{100,3400},
};           

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUCT battery_profile_temperature[] =
{
       {0  , 0 }, 
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },  
	{0  , 0 }, 
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },  
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 }
};    

// ============================================================
// <Rbat, Battery_Voltage> Table
// ============================================================
// T0 -10C
R_PROFILE_STRUCT r_profile_t0[] =
{
{700,4351},   
{706,4308}, 
{731,4279}, 
{727,4254}, 
{714,4233}, 
{709,4212}, 
{700,4194}, 
{688,4176}, 
{681,4159}, 
{688,4143}, 
{682,4128}, 
{673,4113}, 
{650,4100}, 
{661,4086}, 
{655,4074}, 
{650,4061}, 
{636,4049}, 
{630,4036}, 
{638,4024}, 
{633,4012}, 
{618,4000}, 
{619,3989}, 
{616,3979}, 
{623,3969}, 
{619,3960}, 
{613,3951}, 
{612,3942}, 
{619,3933}, 
{618,3925}, 
{609,3917}, 
{619,3909}, 
{612,3901}, 
{613,3893}, 
{612,3886}, 
{622,3878}, 
{616,3871}, 
{618,3865}, 
{627,3858}, 
{621,3852}, 
{631,3846}, 
{633,3840}, 
{626,3834}, 
{629,3828}, 
{633,3823}, 
{632,3818}, 
{649,3813}, 
{641,3808}, 
{646,3804}, 
{663,3800}, 
{667,3796}, 
{661,3793}, 
{665,3789}, 
{671,3786}, 
{687,3783}, 
{679,3780}, 
{684,3777}, 
{701,3774}, 
{709,3771}, 
{711,3768}, 
{717,3765}, 
{709,3763}, 
{705,3760}, 
{712,3757}, 
{739,3754}, 
{744,3751}, 
{748,3749}, 
{752,3745}, 
{732,3743}, 
{735,3740}, 
{745,3737}, 
{774,3734}, 
{779,3731}, 
{778,3728}, 
{788,3725}, 
{784,3722}, 
{770,3718}, 
{774,3715}, 
{771,3711}, 
{813,3707}, 
{787,3703}, 
{786,3699}, 
{785,3695}, 
{829,3690}, 
{802,3685}, 
{803,3679}, 
{801,3674}, 
{805,3654}, 
{804,3618}, 
{805,3587}, 
{806,3564}, 
{807,3542}, 
{808,3507}, 
{809,3476}, 
{815,3438}, 
{817,3398}, 
{819,3384}, 
{821,3373}, 
{822,3361}, 
{823,3347}, 
{824,3335}, 
{825,3322}, 
{827,3307}, 
{823,3302}, 
{827,3292}, 
{835,3287}, 
};      

// T1 0C
R_PROFILE_STRUCT r_profile_t1[] =
{
{410,4347},   
{419,4311}, 
{434,4290}, 
{434,4272}, 
{434,4257}, 
{431,4242}, 
{429,4228}, 
{434,4214}, 
{427,4201}, 
{426,4188}, 
{424,4175}, 
{422,4162}, 
{422,4150}, 
{425,4138}, 
{419,4126}, 
{417,4114}, 
{417,4102}, 
{421,4090}, 
{416,4079}, 
{414,4067}, 
{415,4057}, 
{414,4046}, 
{414,4034}, 
{418,4023}, 
{413,4012}, 
{414,4000}, 
{413,3990}, 
{413,3979}, 
{414,3969}, 
{414,3960}, 
{415,3951}, 
{421,3942}, 
{422,3933}, 
{422,3924}, 
{418,3916}, 
{420,3908}, 
{420,3900}, 
{428,3893}, 
{428,3885}, 
{430,3878}, 
{432,3871}, 
{430,3864}, 
{437,3857}, 
{437,3851}, 
{436,3844}, 
{443,3838}, 
{446,3833}, 
{443,3827}, 
{452,3821}, 
{455,3816}, 
{451,3811}, 
{455,3806}, 
{466,3802}, 
{462,3797}, 
{466,3793}, 
{479,3789}, 
{477,3786}, 
{490,3782}, 
{498,3780}, 
{494,3777}, 
{513,3774}, 
{519,3772}, 
{526,3769}, 
{534,3767}, 
{525,3764}, 
{549,3762}, 
{538,3759}, 
{564,3757}, 
{551,3754}, 
{561,3751}, 
{572,3749}, 
{581,3746}, 
{564,3743}, 
{586,3740}, 
{599,3737}, 
{598,3734}, 
{606,3731}, 
{616,3727}, 
{623,3724}, 
{630,3721}, 
{636,3717}, 
{648,3714}, 
{651,3710}, 
{662,3706}, 
{662,3702}, 
{672,3699}, 
{681,3695}, 
{692,3691}, 
{695,3686}, 
{701,3681}, 
{708,3676}, 
{712,3663}, 
{713,3596}, 
{713,3550}, 
{715,3516}, 
{717,3495}, 
{719,3464}, 
{719,3443}, 
{722,3391}, 
{725,3376}, 
{726,3363}, 
{728,3352}, 
{729,3336}, 
{729,3327}, 
{731,3312}, 
};     

// T2 25C
R_PROFILE_STRUCT r_profile_t2[] =
{
{183,4350},  
{183,4334},
{180,4318},
{179,4304},
{179,4292},
{179,4279},
{178,4267},
{177,4255},
{178,4244},
{179,4233},
{178,4221},
{177,4209},
{177,4198},
{177,4187},
{178,4176},
{177,4165},
{176,4154},
{177,4143},
{177,4132},
{177,4122},
{177,4111},
{177,4101},
{176,4090},
{177,4080},
{176,4069},
{176,4060},
{176,4050},
{175,4040},
{176,4030},
{175,4021},
{176,4011},
{176,4002},
{176,3992},
{177,3983},
{176,3974},
{176,3965},
{176,3956},
{176,3947},
{176,3938},
{176,3929},
{176,3920},
{176,3911},
{176,3902},
{177,3894},
{177,3886},
{178,3878},
{178,3871},
{177,3864},
{177,3857},
{177,3851},
{177,3845},
{178,3840},
{178,3834},
{179,3829},
{179,3825},
{179,3820},
{179,3816},
{179,3811},
{179,3807},
{180,3803},
{181,3799},
{181,3796},
{181,3792},
{182,3789},
{182,3786},
{183,3782},
{183,3779},
{184,3776},
{184,3774},
{184,3771},
{185,3768},
{187,3766},
{187,3763},
{189,3761},
{190,3759},
{191,3757},
{192,3755},
{193,3752},
{195,3749},
{195,3746},
{196,3743},
{196,3740},
{197,3736},
{200,3733},
{200,3729},
{202,3725},
{203,3721},
{205,3715},
{207,3710},
{209,3704},
{212,3698},
{214,3691},
{217,3683},
{221,3676},
{227,3671},
{233,3668},
{239,3664},
{248,3659},
{258,3652},
{274,3642},
{290,3622},
{310,3586},
{337,3536},
{368,3468},
{399,3372},
}; 

// T3 50C
R_PROFILE_STRUCT r_profile_t3[] =
{
{178,4348},   
{178,4314}, 
{177,4301}, 
{176,4288}, 
{176,4276}, 
{175,4264}, 
{175,4252}, 
{175,4241}, 
{175,4228}, 
{174,4217}, 
{175,4206}, 
{175,4195}, 
{174,4184}, 
{174,4173}, 
{173,4162}, 
{174,4151}, 
{174,4140}, 
{174,4130}, 
{175,4119}, 
{173,4108}, 
{175,4098}, 
{175,4088}, 
{175,4078}, 
{175,4068}, 
{175,4058}, 
{175,4048}, 
{176,4039}, 
{175,4029}, 
{176,4020}, 
{175,4011}, 
{175,4002}, 
{175,3994}, 
{176,3985}, 
{175,3977}, 
{175,3969}, 
{176,3960}, 
{176,3953}, 
{175,3944}, 
{176,3936}, 
{175,3928}, 
{176,3920}, 
{176,3912}, 
{175,3902}, 
{176,3893}, 
{175,3884}, 
{175,3874}, 
{176,3866}, 
{175,3858}, 
{175,3851}, 
{175,3845}, 
{175,3840}, 
{176,3834}, 
{176,3829}, 
{175,3824}, 
{176,3820}, 
{176,3815}, 
{176,3811}, 
{176,3807}, 
{176,3803}, 
{176,3799}, 
{176,3795}, 
{176,3792}, 
{176,3789}, 
{176,3785}, 
{176,3782}, 
{177,3779}, 
{176,3776}, 
{176,3773}, 
{177,3771}, 
{177,3768}, 
{177,3766}, 
{177,3763}, 
{178,3760}, 
{177,3756}, 
{177,3752}, 
{177,3746}, 
{178,3741}, 
{179,3738}, 
{179,3734}, 
{179,3730}, 
{179,3726}, 
{180,3722}, 
{179,3718}, 
{179,3715}, 
{180,3711}, 
{180,3707}, 
{180,3702}, 
{181,3696}, 
{180,3690}, 
{181,3685}, 
{181,3679}, 
{181,3670}, 
{182,3664}, 
{183,3662}, 
{182,3660}, 
{185,3657}, 
{184,3653}, 
{185,3647}, 
{186,3636}, 
{188,3610}, 
{191,3568}, 
{196,3514}, 
{199,3443}, 
{203,3342}, 
{214,3165}, 
}; 

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUCT r_profile_temperature[] =
{
  {0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },  
	{0  , 0 }, 
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },  
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 },
	{0  , 0 }
};    

/* ============================================================
// function prototype
// ============================================================*/
int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUCT_P fgauge_get_profile(unsigned int temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUCT_P fgauge_get_profile_r_table(unsigned int temperature);

#endif	/*#ifndef _CUST_BATTERY_METER_TABLE_H*/
