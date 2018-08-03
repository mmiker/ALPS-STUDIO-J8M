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
{0,4319},
{1,4306},
{2,4294},
{3,4283},
{4,4271},
{5,4260},
{6,4249},
{7,4238},
{8,4227},
{9,4216},
{10,4205},
{11,4194},
{12,4183},
{13,4172},
{14,4161},
{15,4150},
{16,4140},
{17,4129},
{18,4119},
{19,4108},
{20,4098},
{21,4088},
{22,4078},
{23,4067},
{24,4058},
{25,4048},
{26,4039},
{27,4029},
{28,4020},
{30,4010},
{31,4001},
{32,3992},
{33,3984},
{34,3975},
{35,3966},
{36,3957},
{37,3948},
{38,3938},
{39,3929},
{40,3917},
{41,3906},
{42,3896},
{43,3886},
{44,3878},
{45,3871},
{46,3864},
{47,3858},
{48,3852},
{49,3846},
{50,3841},
{51,3836},
{52,3831},
{53,3826},
{54,3821},
{55,3817},
{56,3813},
{57,3808},
{58,3804},
{59,3800},
{60,3797},
{61,3793},
{62,3789},
{63,3785},
{64,3782},
{65,3779},
{66,3775},
{67,3772},
{68,3768},
{69,3764},
{70,3759},
{71,3754},
{72,3748},
{73,3743},
{74,3738},
{75,3734},
{76,3729},
{77,3725},
{78,3721},
{79,3717},
{80,3714},
{81,3711},
{82,3706},
{83,3701},
{84,3696},
{85,3691},
{86,3686},
{88,3679},
{89,3672},
{90,3668},
{91,3667},
{92,3665},
{93,3664},
{94,3661},
{95,3656},
{96,3645},
{97,3615},
{98,3570},
{99,3512},
{100,3400}
};      
        
// T1 0C 
BATTERY_PROFILE_STRUCT battery_profile_t1[] =
{
{0,4307},  
{1,4292},  
{2,4280},  
{3,4268},  
{4,4257},  
{5,4246},  
{6,4235},  
{7,4223},  
{8,4213},  
{9,4201},  
{10,4191}, 
{11,4180}, 
{12,4169}, 
{13,4159}, 
{14,4148}, 
{15,4138}, 
{16,4127}, 
{18,4116}, 
{19,4106}, 
{20,4095}, 
{21,4085}, 
{22,4074}, 
{23,4065}, 
{24,4058}, 
{25,4050}, 
{26,4040}, 
{27,4026}, 
{28,4009}, 
{29,3995}, 
{30,3983}, 
{31,3973}, 
{32,3965}, 
{33,3959}, 
{34,3952}, 
{35,3944}, 
{36,3936}, 
{37,3927}, 
{38,3918}, 
{39,3910}, 
{40,3902}, 
{41,3894}, 
{42,3887}, 
{43,3880}, 
{44,3874}, 
{45,3867}, 
{46,3862}, 
{47,3856}, 
{48,3850}, 
{49,3845}, 
{50,3840}, 
{52,3835}, 
{53,3830}, 
{54,3826}, 
{55,3821}, 
{56,3817}, 
{57,3812}, 
{58,3809}, 
{59,3805}, 
{60,3801}, 
{61,3797}, 
{62,3793}, 
{63,3790}, 
{64,3786}, 
{65,3783}, 
{66,3779}, 
{67,3776}, 
{68,3773}, 
{69,3770}, 
{70,3767}, 
{71,3764}, 
{72,3761}, 
{73,3758}, 
{74,3755}, 
{75,3752}, 
{76,3749}, 
{77,3746}, 
{78,3742}, 
{79,3739}, 
{80,3735}, 
{81,3731}, 
{82,3727}, 
{83,3723}, 
{85,3720}, 
{86,3717}, 
{87,3714}, 
{88,3711}, 
{89,3709}, 
{90,3707}, 
{91,3703}, 
{92,3700}, 
{93,3698}, 
{94,3694}, 
{95,3688}, 
{96,3680}, 
{97,3663}, 
{98,3569}, 
{100,3400},
{100,3400},
{100,3400}
};           

// T2 25C
BATTERY_PROFILE_STRUCT battery_profile_t2[] =
{
{0,4307}, 
{1,4292}, 
{2,4280}, 
{3,4268}, 
{4,4257}, 
{5,4246}, 
{6,4235}, 
{7,4223}, 
{8,4213}, 
{9,4201}, 
{10,4191},
{11,4180},
{12,4169},
{13,4159},
{14,4148},
{15,4138},
{16,4127},
{17,4116},
{18,4106},
{19,4095},
{20,4085},
{21,4074},
{22,4065},
{23,4058},
{24,4050},
{25,4040},
{26,4026},
{27,4009},
{28,3995},
{29,3983},
{30,3973},
{31,3965},
{32,3959},
{33,3952},
{34,3944},
{35,3936},
{36,3927},
{37,3918},
{38,3910},
{39,3902},
{40,3894},
{41,3887},
{42,3880},
{43,3874},
{44,3867},
{45,3862},
{47,3856},
{48,3850},
{49,3845},
{50,3840},
{51,3835},
{52,3830},
{53,3826},
{54,3821},
{55,3817},
{56,3812},
{57,3809},
{58,3805},
{59,3801},
{60,3797},
{61,3793},
{62,3790},
{63,3786},
{64,3783},
{65,3779},
{66,3776},
{67,3773},
{68,3770},
{69,3767},
{70,3764},
{71,3761},
{72,3758},
{73,3755},
{74,3752},
{75,3749},
{76,3746},
{77,3742},
{78,3739},
{79,3735},
{80,3731},
{81,3729},
{82,3727},
{83,3724},
{84,3721},
{85,3718},
{86,3716},
{87,3714},
{88,3712},
{89,3710},
{90,3707},
{91,3706},
{92,3705},
{93,3701},
{94,3700},
{95,3696},
{96,3680},
{97,3663},
{98,3603},
{100,3400}
};     

// T3 50C
BATTERY_PROFILE_STRUCT battery_profile_t3[] =
{
{0,4319}, 
{1,4306}, 
{2,4294}, 
{3,4283}, 
{4,4271}, 
{5,4260}, 
{6,4249}, 
{7,4238}, 
{8,4227}, 
{9,4216}, 
{10,4205},
{11,4194},
{12,4183},
{13,4172},
{14,4161},
{15,4150},
{16,4140},
{17,4129},
{18,4119},
{19,4108},
{20,4098},
{21,4088},
{22,4078},
{23,4067},
{24,4058},
{26,4048},
{27,4039},
{28,4029},
{29,4020},
{30,4010},
{31,4001},
{32,3992},
{33,3984},
{34,3975},
{35,3966},
{36,3957},
{37,3948},
{38,3938},
{39,3929},
{40,3917},
{41,3906},
{42,3896},
{43,3886},
{44,3878},
{45,3871},
{46,3864},
{47,3858},
{48,3852},
{49,3846},
{50,3841},
{51,3836},
{52,3831},
{53,3826},
{54,3821},
{55,3817},
{56,3813},
{57,3808},
{58,3804},
{59,3800},
{60,3797},
{61,3793},
{62,3789},
{63,3785},
{64,3782},
{65,3779},
{66,3775},
{67,3772},
{68,3768},
{69,3764},
{70,3759},
{71,3757},
{72,3754},
{73,3751},
{74,3749},
{76,3746},
{77,3744},
{78,3741},
{79,3739},
{80,3737},
{81,3734},
{82,3732},
{83,3729},
{84,3726},
{85,3724},
{86,3721},
{87,3719},
{88,3717},
{89,3714},
{90,3711},
{91,3708},
{92,3706},
{93,3703},
{94,3700},
{95,3695},
{96,3688},
{97,3683},
{98,3663},
{99,3583},
{100,3400}
};           

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUCT battery_profile_temperature[] =
{
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0}
};    

// ============================================================
// <Rbat, Battery_Voltage> Table
// ============================================================
// T0 -10C
R_PROFILE_STRUCT r_profile_t0[] =
{
{813,4319},
{911,4306},
{890,4294},
{866,4283},
{816,4271},
{827,4260},
{807,4249},
{771,4238},
{778,4227},
{750,4216},
{741,4205},
{747,4194},
{738,4183},
{720,4172},
{714,4161},
{707,4150},
{699,4140},
{710,4129},
{696,4119},
{697,4108},
{683,4098},
{683,4088},
{681,4078},
{687,4067},
{679,4058},
{679,4048},
{670,4039},
{669,4029},
{682,4020},
{671,4010},
{670,4001},
{679,3992},
{670,3984},
{671,3975},
{682,3966},
{674,3957},
{700,3948},
{675,3938},
{676,3929},
{679,3917},
{691,3906},
{681,3896},
{683,3886},
{693,3878},
{687,3871},
{690,3864},
{688,3858},
{704,3852},
{706,3846},
{709,3841},
{696,3836},
{700,3831},
{701,3826},
{717,3821},
{729,3817},
{706,3813},
{710,3808},
{713,3804},
{716,3800},
{715,3797},
{714,3793},
{745,3789},
{719,3785},
{723,3782},
{726,3779},
{727,3775},
{758,3772},
{729,3768},
{762,3764},
{734,3759},
{766,3754},
{744,3748},
{746,3743},
{741,3738},
{778,3734},
{751,3729},
{781,3725},
{751,3721},
{785,3717},
{787,3714},
{756,3711},
{792,3706},
{758,3701},
{762,3696},
{764,3691},
{768,3686},
{761,3679},
{804,3672},
{765,3668},
{806,3667},
{807,3665},
{766,3664},
{765,3661},
{775,3656},
{772,3645},
{768,3615},
{769,3570},
{772,3512},
{782,3400}
};      

// T1 0C
R_PROFILE_STRUCT r_profile_t1[] =
{
{534,4307}, 
{534,4292}, 
{535,4280}, 
{530,4268}, 
{524,4257}, 
{524,4246}, 
{518,4235}, 
{513,4223}, 
{513,4213}, 
{507,4201}, 
{503,4191}, 
{504,4180}, 
{499,4169}, 
{497,4159}, 
{495,4148}, 
{492,4138}, 
{490,4127}, 
{486,4116}, 
{484,4106}, 
{486,4095}, 
{486,4085}, 
{487,4074}, 
{489,4065}, 
{485,4058}, 
{480,4050}, 
{475,4040}, 
{468,4026}, 
{462,4009}, 
{459,3995}, 
{457,3983}, 
{454,3973}, 
{456,3965}, 
{455,3959}, 
{457,3952}, 
{458,3944}, 
{456,3936}, 
{457,3927}, 
{457,3918}, 
{455,3910}, 
{455,3902}, 
{454,3894}, 
{454,3887}, 
{454,3880}, 
{454,3874}, 
{454,3867}, 
{454,3862}, 
{455,3856}, 
{454,3850}, 
{456,3845}, 
{456,3840}, 
{456,3835}, 
{458,3830}, 
{458,3826}, 
{459,3821}, 
{459,3817}, 
{462,3812}, 
{461,3809}, 
{466,3805}, 
{466,3801}, 
{465,3797}, 
{469,3793}, 
{469,3790}, 
{472,3786}, 
{478,3783}, 
{478,3779}, 
{480,3776}, 
{485,3773}, 
{490,3770}, 
{494,3767}, 
{500,3764}, 
{504,3761}, 
{511,3758}, 
{517,3755}, 
{525,3752}, 
{532,3749}, 
{541,3746}, 
{551,3742}, 
{563,3739}, 
{571,3735}, 
{584,3731}, 
{597,3727}, 
{608,3723}, 
{621,3720}, 
{633,3717}, 
{647,3714}, 
{665,3711}, 
{679,3709}, 
{697,3707}, 
{716,3703}, 
{738,3700}, 
{763,3698}, 
{789,3694}, 
{823,3688}, 
{857,3680}, 
{903,3663}, 
{953,3569}, 
{1019,3400},
{1110,3400},
{1220,3400}
};     

// T2 25C
R_PROFILE_STRUCT r_profile_t2[] =
{
{197,4307},
{197,4292},
{197,4280},
{198,4268},
{197,4257},
{197,4246},
{197,4235},
{197,4223},
{197,4213},
{198,4201},
{198,4191},
{198,4180},
{199,4169},
{198,4159},
{199,4148},
{199,4138},
{200,4127},
{200,4116},
{200,4106},
{200,4095},
{200,4085},
{201,4074},
{204,4065},
{207,4058},
{203,4050},
{203,4040},
{201,4026},
{200,4009},
{200,3995},
{199,3983},
{197,3973},
{198,3965},
{197,3959},
{196,3952},
{193,3944},
{192,3936},
{190,3927},
{188,3918},
{188,3910},
{186,3902},
{186,3894},
{186,3887},
{185,3880},
{185,3874},
{185,3867},
{185,3862},
{184,3856},
{185,3850},
{185,3845},
{185,3840},
{185,3835},
{184,3830},
{186,3826},
{185,3821},
{184,3817},
{186,3812},
{186,3809},
{186,3805},
{186,3801},
{186,3797},
{185,3793},
{186,3790},
{186,3786},
{186,3783},
{186,3779},
{186,3776},
{185,3773},
{186,3770},
{186,3767},
{186,3764},
{186,3761},
{186,3758},
{186,3755},
{185,3752},
{185,3749},
{186,3746},
{186,3742},
{187,3739},
{188,3735},
{189,3731},
{189,3727},
{190,3721},
{192,3716},
{191,3711},
{193,3705},
{192,3698},
{193,3691},
{195,3688},
{199,3686},
{201,3683},
{206,3681},
{211,3677},
{212,3670},
{211,3654},
{217,3619},
{224,3569},
{236,3502},
{270,3400},
{625,3400}   
}; 

// T3 50C
R_PROFILE_STRUCT r_profile_t3[] =
{
{149,4319},
{149,4306},
{149,4294},
{150,4283},
{149,4271},
{149,4260},
{150,4249},
{150,4238},
{150,4227},
{150,4216},
{152,4205},
{151,4194},
{152,4183},
{151,4172},
{152,4161},
{152,4150},
{153,4140},
{153,4129},
{153,4119},
{154,4108},
{154,4098},
{154,4088},
{155,4078},
{156,4067},
{155,4058},
{156,4048},
{157,4039},
{157,4029},
{158,4020},
{159,4010},
{159,4001},
{159,3992},
{161,3984},
{161,3975},
{161,3966},
{162,3957},
{161,3948},
{161,3938},
{159,3929},
{155,3917},
{152,3906},
{151,3896},
{150,3886},
{149,3878},
{148,3871},
{149,3864},
{149,3858},
{149,3852},
{148,3846},
{149,3841},
{150,3836},
{150,3831},
{149,3826},
{151,3821},
{151,3817},
{151,3813},
{152,3808},
{152,3804},
{152,3800},
{153,3797},
{153,3793},
{153,3789},
{153,3785},
{153,3782},
{154,3779},
{153,3775},
{154,3772},
{152,3768},
{152,3764},
{151,3759},
{149,3754},
{149,3748},
{150,3743},
{150,3738},
{149,3734},
{149,3729},
{149,3725},
{149,3721},
{149,3717},
{151,3714},
{151,3711},
{151,3706},
{151,3701},
{151,3696},
{152,3691},
{152,3686},
{150,3679},
{149,3672},
{151,3668},
{152,3667},
{156,3665},
{157,3664},
{159,3661},
{160,3656},
{155,3645},
{155,3615},
{159,3570},
{161,3512},
{168,3400}
}; 

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUCT r_profile_temperature[] =
{
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0}
};    

/* ============================================================
// function prototype
// ============================================================*/
int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUCT_P fgauge_get_profile(unsigned int temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUCT_P fgauge_get_profile_r_table(unsigned int temperature);

#endif	/*#ifndef _CUST_BATTERY_METER_TABLE_H*/

