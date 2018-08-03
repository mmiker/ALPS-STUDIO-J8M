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
	{0  , 4385},         
	{2  , 4355},         
	{4  , 4328},         
	{5  , 4301},         
	{7  , 4275},         
	{9  , 4252},         
	{11  , 4229},         
	{13  , 4205},         
	{14  , 4184},         
	{16  , 4164},         
	{18  , 4143},         
	{20  , 4123},         
	{21  , 4103},         
	{23  , 4085},         
	{25  , 4067},         
	{27  , 4049},         
	{29  , 4024},         
	{30  , 3998},         
	{32  , 3974},         
	{34  , 3953},         
	{36  , 3940},         
	{38  , 3925},         
	{39  , 3912},         
	{41  , 3900},         
	{43  , 3886},         
	{45  , 3875},         
	{47  , 3863},         
	{48  , 3852},         
	{50  , 3842},         
	{52  , 3832},         
	{54  , 3823},         
	{56  , 3814},         
	{57  , 3806},         
	{59  , 3798},         
	{61  , 3793},         
	{63  , 3787},         
	{64  , 3783},         
	{66  , 3779},         
	{68  , 3775},         
	{70  , 3772},         
	{72  , 3769},         
	{73  , 3764},         
	{75  , 3760},         
	{77  , 3754},         
	{79  , 3748},         
	{81  , 3741},         
	{82  , 3736},         
	{83  , 3733},         
	{84  , 3729},         
	{85  , 3725},         
	{86  , 3720},         
	{86  , 3718},         
	{87  , 3714},         
	{87  , 3712},         
	{88  , 3709},         
	{88  , 3705},         
	{89  , 3703},         
	{89  , 3700},         
	{90  , 3698},         
	{90  , 3697},         
	{90  , 3695},          
	{91  , 3692},
	{91  , 3692},
	{92  , 3690},
	{92  , 3689},
	{92  , 3688},
	{92  , 3688},
	{93  , 3687},
	{93  , 3687},
	{93  , 3686},
	{94  , 3684}, 
	{94  , 3684},
	{94  , 3683},
	{94  , 3683},
	{94  , 3682},
	{95  , 3682}, 
	{95  , 3681},
	{95  , 3681},
	{95  , 3679},
	{96  , 3678},
	{96  , 3678},
	{96  , 3677},
	{96  , 3676},
	{96  , 3674},
	{96  , 3673},
	{97  , 3671},
	{97  , 3667},
	{97  , 3661},
	{97  , 3657},
	{97  , 3653},
	{97  , 3650},
	{98  , 3643},
	{98  , 3637},
	{98  , 3632},
	{98  , 3628},
	{98  , 3625},
	{98  , 3622},
	{98  , 3619},
	{98  , 3615},
	{98  , 3612},	       
	{98  , 3610},
    {98  , 3607},
    {98  , 3602},
    {98  , 3600},
    {99  , 3597},
    {99  , 3595},
    {99  , 3593},
    {99  , 3591},
    {99  , 3589},
    {99  , 3586},
    {99  , 3585},
    {99  , 3584},
    {99  , 3583},
    {99  , 3580},
    {99  , 3579},
    {99  , 3575},
    {99  , 3574},
    {99  , 3573},
    {99  , 3571},
    {99  , 3570},
    {99  , 3569},
    {99  , 3568},
    {99  , 3566},
    {99  , 3565},
    {99  , 3563},
    {99  , 3562},
    {99  , 3560},
    {99  , 3559},
    {99  , 3558},
    {99  , 3557},
    {99  , 3555},
    {99  , 3554},
    {99  , 3553},
    {99  , 3552},
    {99  , 3550},
    {99  , 3548},
    {99  , 3547},
    {99  , 3545},
    {99  , 3544},
    {99  , 3543},
    {99  , 3543},
    {99  , 3542},
    {99  , 3542},
    {99  , 3539},
    {99  , 3538},
    {99  , 3537},
    {99  , 3535},
    {99  , 3533},
    {99  , 3533},
    {99  , 3532},
    {99  , 3531},
    {99  , 3528},
    {99  , 3527},
    {99  , 3524},
    {99  , 3524},
    {99  , 3522},
    {99  , 3521},
    {99  , 3517},
    {99  , 3516},
    {99  , 3514},
    {99  , 3513},
    {99  , 3511},
    {99  , 3509},
    {99  , 3507},
    {99  , 3506},
    {99  , 3504},
    {99  , 3502},
    {99  , 3501},
    {99  , 3498},
    {99  , 3497},
    {100 , 3400}
};      
        
// T1 0C 
BATTERY_PROFILE_STRUCT battery_profile_t1[] =
{
 {0  , 4395},         
	{2  , 4370},         
	{3  , 4349},         
	{5  , 4328},         
	{7  , 4308},         
	{9  , 4287},         
	{10  , 4266},         
	{12  , 4246},         
	{14  , 4226},         
	{16  , 4205},         
	{17  , 4187},         
	{19  , 4167},         
	{21  , 4148},         
	{22  , 4129},         
	{24  , 4110},         
	{26  , 4091},         
	{28  , 4074},         
	{29  , 4058},         
	{31  , 4036},         
	{33  , 4009},         
	{35  , 3983},         
	{36  , 3962},         
	{38  , 3946},         
	{40  , 3932},         
	{41  , 3920},         
	{43  , 3906},         
	{45  , 3893},         
	{47  , 3880},         
	{48  , 3869},         
	{50  , 3857},         
	{52  , 3848},         
	{53  , 3839},         
	{55  , 3831},         
	{57  , 3822},         
	{59  , 3814},         
	{60  , 3807},         
	{62  , 3801},         
	{64  , 3793},         
	{66  , 3788},         
	{67  , 3783},         
	{69  , 3780},         
	{71  , 3776},         
	{72  , 3774},         
	{74  , 3769},         
	{76  , 3764},         
	{78  , 3759},         
	{79  , 3752},         
	{81  , 3745},         
	{83  , 3735},         
	{85  , 3726},         
	{86  , 3717},         
	{88  , 3704},         
	{90  , 3697},         
	{91  , 3690},         
	{93  , 3684},         
	{95  , 3671},         
	{96  , 3651},         
	{97  , 3626},         
	{97  , 3599},         
	{98  , 3573},         
	{98  , 3550},          
	{98  , 3529},
	{99  , 3512},
	{99  , 3495},
	{99  , 3482},
	{99  , 3470},
	{99  , 3460},
	{99  , 3451},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},	       
	{100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400}   
};           

// T2 25C
BATTERY_PROFILE_STRUCT battery_profile_t2[] =
{
	{0  , 4390},         
	{2  , 4368},         
	{4  , 4350},         
	{5  , 4330},         
	{7  , 4309},         
	{9  , 4289},         
	{11  , 4268},         
	{12  , 4247},         
	{14  , 4229},         
	{16  , 4208},         
	{18  , 4187},         
	{19  , 4168},         
	{21  , 4148},         
	{23  , 4128},         
	{25  , 4110},         
	{26  , 4090},         
	{28  , 4071},         
	{30  , 4056},         
	{32  , 4038},         
	{33  , 4018},         
	{35  , 4000},         
	{37  , 3987},         
	{39  , 3972},         
	{40  , 3955},         
	{42  , 3937},         
	{44  , 3915},         
	{46  , 3896},         
	{47  , 3881},         
	{49  , 3870},         
	{51  , 3859},         
	{53  , 3849},         
	{54  , 3842},         
	{56  , 3832},         
	{58  , 3824},         
	{60  , 3817},         
	{61  , 3811},         
	{63  , 3803},         
	{65  , 3798},         
	{67  , 3793},         
	{68  , 3788},         
	{70  , 3783},         
	{72  , 3776},         
	{74  , 3769},         
	{75  , 3761},         
	{77  , 3755},         
	{79  , 3746},         
	{81  , 3741},         
	{82  , 3734},         
	{84  , 3725},         
	{86  , 3714},         
	{88  , 3703},         
	{89  , 3690},         
	{91  , 3688},         
	{93  , 3687},         
	{95  , 3682},         
	{96  , 3652},         
	{98  , 3568},         
	{100 , 3400},         
	{100 , 3400},         
	{100 , 3400},         
	{100 , 3400},          
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},	       
	{100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400}    
};     

// T3 50C
BATTERY_PROFILE_STRUCT battery_profile_t3[] =
{
	{0  , 4390},         
	{2  , 4371},         
	{3  , 4352},         
	{5  , 4334},         
	{7  , 4315},         
	{9  , 4294},         
	{10  , 4274},         
	{12  , 4254},         
	{14  , 4234},         
	{16  , 4213},         
	{17  , 4193},         
	{19  , 4173},         
	{21  , 4154},         
	{23  , 4134},         
	{24  , 4115},         
	{26  , 4096},         
	{28  , 4078},         
	{30  , 4061},         
	{31  , 4043},         
	{33  , 4027},         
	{35  , 4010},         
	{37  , 3995},         
	{38  , 3979},         
	{40  , 3963},         
	{42  , 3946},         
	{44  , 3923},         
	{45  , 3903},         
	{47  , 3889},         
	{49  , 3877},         
	{51  , 3866},         
	{52  , 3856},         
	{54  , 3846},         
	{56  , 3837},         
	{58  , 3829},         
	{59  , 3821},         
	{61  , 3815},         
	{63  , 3807},         
	{65  , 3801},         
	{66  , 3795},         
	{68  , 3790},         
	{70  , 3784},         
	{72  , 3773},         
	{73  , 3760},         
	{75  , 3752},         
	{77  , 3744},         
	{79  , 3736},         
	{80  , 3730},         
	{82  , 3722},         
	{84  , 3713},         
	{86  , 3703},         
	{87  , 3693},         
	{89  , 3681},         
	{91  , 3679},         
	{93  , 3677},         
	{94  , 3672},         
	{96  , 3645},         
	{98  , 3569},         
	{100 , 3400},         
	{100 , 3400},         
	{100 , 3400},         
	{100 , 3400},          
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400}, 
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},
	{100 , 3400},	       
	{100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400},
    {100 , 3400}    
};           

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUCT battery_profile_temperature[] =
{
    {0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},          
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0}, 
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0}, 
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},	       
	{0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0} 
};    

// ============================================================
// <Rbat, Battery_Voltage> Table
// ============================================================
// T0 -10C
R_PROFILE_STRUCT r_profile_t0[] =
{
	{1265 , 4385},         
	{1265 , 4355},         
	{1277 , 4328},         
	{1290 , 4301},         
	{1263 , 4275},         
	{1243 , 4252},         
	{1223 , 4229},         
	{1193 , 4205},         
	{1173 , 4184},         
	{1150 , 4164},         
	{1133 , 4143},         
	{1110 , 4123},         
	{1088 , 4103},         
	{1075 , 4085},         
	{1065 , 4067},         
	{1058 , 4049},         
	{1030 , 4024},         
	{1005 , 3998},         
	{983  , 3974},         
	{963  , 3953},         
	{965  , 3940},         
	{963  , 3925},         
	{958  , 3912},         
	{956  , 3900},         
	{955  , 3886},         
	{955  , 3875},         
	{955  , 3863},         
	{955  , 3852},         
	{958  , 3842},         
	{958  , 3832},         
	{963  , 3823},         
	{970  , 3814},         
	{975  , 3806},         
	{980  , 3798},         
	{990  , 3793},         
	{1000 , 3787},         
	{1013 , 3783},         
	{1028 , 3779},         
	{1045 , 3775},         
	{1073 , 3772},         
	{1105 , 3769},         
	{1135 , 3764},         
	{1175 , 3760},         
	{1223 , 3754},         
	{1273 , 3748},         
	{1323 , 3741},         
	{1328 , 3736},         
	{1333 , 3733},         
	{1323 , 3729},         
	{1313 , 3725},         
	{1300 , 3720},         
	{1295 , 3718},         
	{1285 , 3714},         
	{1280 , 3712},         
	{1273 , 3709},         
	{1263 , 3705},         
	{1258 , 3703},         
	{1250 , 3700},         
	{1245 , 3698},         
	{1243 , 3697},         
	{1238 , 3695},          
	{1230 , 3692},
	{1230 , 3692},
	{1225 , 3690},
	{1223 , 3689},
	{1220 , 3688},
	{1220 , 3688},
	{1218 , 3687},
	{1218 , 3687},
	{1215 , 3686},
	{1210 , 3684}, 
	{1210 , 3684},
	{1208 , 3683},
	{1208 , 3683},
	{1205 , 3682},
	{1205 , 3682}, 
	{1203 , 3681},
	{1203 , 3681},
	{1198 , 3679},
	{1195 , 3678},
	{1195 , 3678},
	{1193 , 3677},
	{1190 , 3676},
	{1185 , 3674},
	{1183 , 3673},
	{1178 , 3671},
	{1168 , 3667},
	{1153 , 3661},
	{1143 , 3657},
	{1133 , 3653},
	{1125 , 3650},
	{1108 , 3643},
	{1093 , 3637},
	{1080 , 3632},
	{1070 , 3628},
	{1063 , 3625},
	{1055 , 3622},
	{1048 , 3619},
	{1038 , 3615},
	{1030 , 3612},	       
	{1025 , 3610},
    {1018 , 3607},
    {1005 , 3602},
    {1000 , 3600},
    {993  , 3597},
    {988  , 3595},
    {983  , 3593},
    {978  , 3591},
    {973  , 3589},
    {965  , 3586},
    {963  , 3585},
    {960  , 3584},
    {958  , 3583},
    {950  , 3580},
    {948  , 3579},
    {938  , 3575},
    {935  , 3574},
    {933  , 3573},
    {928  , 3571},
    {925  , 3570},
    {923  , 3569},
    {920  , 3568},
    {915  , 3566},
    {913  , 3565},
    {910  , 3563},
    {908  , 3562},
    {903  , 3560},
    {900  , 3559},
    {898  , 3558},
    {895  , 3557},
    {888  , 3555},
    {885  , 3554},
    {885  , 3553},
    {883  , 3552},
    {875  , 3550},
    {873  , 3548},
    {868  , 3547},
    {863  , 3545},
    {860  , 3544},
    {860  , 3543},
    {860  , 3543},
    {858  , 3542},
    {858  , 3542},
    {850  , 3539},
    {848  , 3538},
    {848  , 3537},
    {843  , 3535},
    {838  , 3533},
    {835  , 3533},
    {833  , 3532},
    {830  , 3531},
    {823  , 3528},
    {820  , 3527},
    {813  , 3524},
    {813  , 3524},
    {810  , 3522},
    {808  , 3521},
    {798  , 3517},
    {795  , 3516},
    {790  , 3514},
    {790  , 3513},
    {783  , 3511},
    {780  , 3509},
    {773  , 3507},
    {773  , 3506},
    {765  , 3504},
    {763  , 3502},
    {760  , 3501},
    {753  , 3498},
    {750  , 3497},
    {658  , 3400}    
};      

// T1 0C
R_PROFILE_STRUCT r_profile_t1[] =
{
	{660  , 4395},         
	{660  , 4370},         
	{657  , 4349},         
	{643  , 4328},         
	{635  , 4308},         
	{628  , 4287},         
	{615  , 4266},         
	{608  , 4246},         
	{595  , 4226},         
	{585  , 4205},         
	{580  , 4187},         
	{573  , 4167},         
	{565  , 4148},         
	{560  , 4129},         
	{555  , 4110},         
	{545  , 4091},         
	{548  , 4074},         
	{550  , 4058},         
	{537  , 4036},         
	{515  , 4009},         
	{495  , 3983},         
	{483  , 3962},         
	{478  , 3946},         
	{473  , 3932},         
	{475  , 3920},         
	{468  , 3906},         
	{465  , 3893},         
	{463  , 3880},         
	{463  , 3869},         
	{463  , 3857},         
	{463  , 3848},         
	{470  , 3839},         
	{475  , 3831},         
	{475  , 3822},         
	{473  , 3814},         
	{483  , 3807},         
	{488  , 3801},         
	{485  , 3793},         
	{495  , 3788},         
	{500  , 3783},         
	{510  , 3780},         
	{515  , 3776},         
	{530  , 3774},         
	{543  , 3769},         
	{563  , 3764},         
	{583  , 3759},         
	{610  , 3752},         
	{638  , 3745},         
	{665  , 3735},         
	{705  , 3726},         
	{753  , 3717},         
	{788  , 3704},         
	{860  , 3697},         
	{938  , 3690},         
	{1048 , 3684},         
	{1178 , 3671},         
	{1128 , 3651},         
	{1065 , 3626},         
	{998  , 3599},         
	{933  , 3573},         
	{875  , 3550},          
	{823  , 3529},
	{780  , 3512},
	{738  , 3495},
	{705  , 3482},
	{675  , 3470},
	{650  , 3460},
	{628  , 3451},
	{612  , 3400},
	{595  , 3400},
	{583  , 3400}, 
	{570  , 3400},
	{563  , 3400},
	{553  , 3400},
	{548  , 3400},
	{535  , 3400}, 
	{533  , 3400},
	{528  , 3400},
	{523  , 3400},
	{515  , 3400},
	{513  , 3400},
	{510  , 3400},
	{508  , 3400},
	{505  , 3400},
	{508  , 3400},
	{498  , 3400},
	{495  , 3400},
	{488  , 3400},
	{483  , 3400},
	{480  , 3400},
	{473  , 3400},
	{470  , 3400},
	{463  , 3400},
	{460  , 3400},
	{453  , 3400},
	{448  , 3400},
	{440  , 3400},
	{438  , 3400},
	{433  , 3400},
	{425  , 3400},	       
	{420  , 3400},
    {413  , 3400},
    {410  , 3400},
    {400  , 3400},
    {395  , 3400},
    {388  , 3400},
    {383  , 3400},
    {375  , 3400},
    {370  , 3400},
    {363  , 3400},
    {360  , 3400},
    {350  , 3400},
    {345  , 3400},
    {338  , 3400},
    {328  , 3400},
    {320  , 3400},
    {323  , 3400},
    {313  , 3400},
    {305  , 3400},
    {300  , 3400},
    {295  , 3400},
    {298  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293  , 3400},
    {293 , 3400}
};     

// T2 25C
R_PROFILE_STRUCT r_profile_t2[] =
{
	{140 , 4390},         
	{140 , 4368},         
	{145 , 4350},         
	{145 , 4330},         
	{144 , 4309},         
	{143 , 4289},         
	{143 , 4268},         
	{143 , 4247},         
	{148 , 4229},         
	{150 , 4208},         
	{150 , 4187},         
	{150 , 4168},         
	{153 , 4148},         
	{155 , 4128},         
	{155 , 4110},         
	{155 , 4090},         
	{157 , 4071},         
	{163 , 4056},         
	{168 , 4038},         
	{170 , 4018},         
	{170 , 4000},         
	{180 , 3987},         
	{178 , 3972},         
	{175 , 3955},         
	{168 , 3937},         
	{153 , 3915},         
	{138 , 3896},         
	{135 , 3881},         
	{135 , 3870},         
	{135 , 3859},         
	{135 , 3849},         
	{140 , 3842},         
	{140 , 3832},         
	{140 , 3824},         
	{140 , 3817},         
	{143 , 3811},         
	{143 , 3803},         
	{145 , 3798},         
	{148 , 3793},         
	{150 , 3788},         
	{150 , 3783},         
	{148 , 3776},         
	{143 , 3769},         
	{140 , 3761},         
	{140 , 3755},         
	{139 , 3746},         
	{138 , 3741},         
	{140 , 3734},         
	{140 , 3725},         
	{140 , 3714},         
	{140 , 3703},         
	{142 , 3690},         
	{143 , 3688},         
	{153 , 3687},         
	{165 , 3682},         
	{165 , 3652},         
	{178 , 3568},         
	{218 , 3400},         
	{263 , 3400},         
	{308 , 3400},         
	{208 , 3400},          
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400}, 
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400}, 
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},
	{208 , 3400},	       
	{208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400},
    {208 , 3400}       
}; 

// T3 50C
R_PROFILE_STRUCT r_profile_t3[] =
{
	{100 , 4390},         
	{100 , 4371},         
	{100 , 4352},         
	{100 , 4334},         
	{100 , 4315},         
	{100 , 4294},         
	{100 , 4274},         
	{100 , 4254},         
	{100 , 4234},         
	{100 , 4213},         
	{100 , 4193},         
	{100 , 4173},         
	{103 , 4154},         
	{103 , 4134},         
	{103 , 4115},         
	{103 , 4096},         
	{108 , 4078},         
	{110 , 4061},         
	{110 , 4043},         
	{115 , 4027},         
	{118 , 4010},         
	{120 , 3995},         
	{123 , 3979},         
	{125 , 3963},         
	{125 , 3946},         
	{110 , 3923},         
	{100 , 3903},         
	{100 , 3889},         
	{100 , 3877},         
	{100 , 3866},         
	{100 , 3856},         
	{100 , 3846},         
	{100 , 3837},         
	{100 , 3829},         
	{103 , 3821},         
	{108 , 3815},         
	{105 , 3807},         
	{110 , 3801},         
	{110 , 3795},         
	{115 , 3790},         
	{115 , 3784},         
	{108 , 3773},         
	{100 , 3760},         
	{103 , 3752},         
	{103 , 3744},         
	{103 , 3736},         
	{103 , 3730},         
	{103 , 3722},         
	{103 , 3713},         
	{103 , 3703},         
	{105 , 3693},         
	{98  , 3681},         
	{103 , 3679},         
	{108 , 3677},         
	{115 , 3672},         
	{110 , 3645},         
	{118 , 3569},         
	{135 , 3400},         
	{175 , 3400},         
	{130 , 3400},         
	{103 , 3400},          
	{88  , 3400},
	{85  , 3400},
	{78  , 3400},
	{78  , 3400},
	{75  , 3400},
	{70  , 3400},
	{68  , 3400},
	{65  , 3400},
	{58  , 3400},
	{55  , 3400}, 
	{50  , 3400},
	{48  , 3400},
	{43  , 3400},
	{38  , 3400},
	{30  , 3400}, 
	{25  , 3400},
	{20  , 3400},
	{13  , 3400},
	{8  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},
	{0  , 3400},	       
	{0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400},
    {0  , 3400}   
}; 

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUCT r_profile_temperature[] =
{
    {0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},         
	{0  , 0},          
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0}, 
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0}, 
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},
	{0  , 0},	       
	{0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0},
    {0  , 0}  
};    

/* ============================================================
// function prototype
// ============================================================*/
int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUCT_P fgauge_get_profile(unsigned int temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUCT_P fgauge_get_profile_r_table(unsigned int temperature);

#endif	/*#ifndef _CUST_BATTERY_METER_TABLE_H*/
