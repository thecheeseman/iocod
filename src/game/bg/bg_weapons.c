#include "bg_local.h"

const char *weapon_type_str[] = {
	"bullet",
	"grenade",
	"projectile"
};
const int weapon_type_size = ARRAY_SIZE(weapon_type_str);

const char *overlay_reticle_str[] = {
	"none",
	"crosshair",
	"FG42",
	"Springfield",
	"Gewehr43"
};
const int overlay_reticle_size = ARRAY_SIZE(overlay_reticle_str);

const char *weapon_class_str[] = {
	"rifle",
	"mg",
	"smg",
	"pistol",
	"grenade",
	"rocketlauncher",
	"turret",
	"non-player"
};
const int weapon_class_size = ARRAY_SIZE(weapon_class_str);

const char *weapon_slot_str[] = {
	"none",
	"primary",
	"primaryb",
	"pistol",
	"grenade",
	"smokegrenade"
};
const int weapon_slot_size = ARRAY_SIZE(weapon_slot_str);

const char *weapon_stance_str[] = {
	"stand",
	"duck",
	"prone"
};
const int weapon_stance_size = ARRAY_SIZE(weapon_stance_str);

const char *projectile_type_str[] = {
	"grenade",
	"rocket",
	"molotov",
	"none"
};
const int projectile_type_size = ARRAY_SIZE(projectile_type_str);

//
struct weapon_config_string {
	char *string;
	int offset;
	int type;
};


// because of stylistic reasons, the structure data names have
// underscores instead of camelCase like the original game
// the simpler option is just to have the structure match the camelCase
// names, but that wouldn't match with the rest of the project's style

struct weapon_config_string config_strings[] = {
	#define ENTRY(n, x, t, o) \
	{ \
		#n, \
		(intptr_t)&((struct weapon *)0)->x, \
		t \
	},

	// offsets in the rightmost column match original code

	ENTRY(displayName,				display_name,			WFT_STRING,	0x08)
	ENTRY(AIOverlayDescription,		ai_overlay_description,	WFT_STRING,	0x0c)

	ENTRY(modeName,					mode_name,				WFT_STRING,	0x6c)
	ENTRY(gunModel,					gun_model,				WFT_STRING,	0x10)
	ENTRY(handModel,				hand_model,				WFT_STRING,	0x14)

	ENTRY(idleAnim,					idle_anim,				WFT_STRING,	0x1c)
	ENTRY(emptyIdleAnim,			empty_idle_anim,		WFT_STRING,	0x20)
	ENTRY(fireAnim,					fire_anim,				WFT_STRING,	0x24)
	ENTRY(holdFireAnim,				hold_fire_anim,			WFT_STRING,	0x28)
	ENTRY(lastShotAnim,				last_shot_anim,			WFT_STRING,	0x2c)
	ENTRY(rechamberAnim,			rechamber_anim,			WFT_STRING,	0x30)
	ENTRY(meleeAnim,				melee_anim,				WFT_STRING,	0x34)
	ENTRY(reloadAnim,				reload_anim,			WFT_STRING,	0x38)
	ENTRY(reloadEmptyAnim,			reload_empty_anim,		WFT_STRING,	0x3c)
	ENTRY(reloadStartAnim,			reload_start_anim,		WFT_STRING,	0x40)
	ENTRY(reloadEndAnim,			reload_end_anim,		WFT_STRING,	0x44)
	ENTRY(raiseAnim,				raise_anim,				WFT_STRING,	0x48)
	ENTRY(dropAnim,					drop_anim,				WFT_STRING,	0x4c)
	ENTRY(altRaiseAnim,				alt_raise_anim,			WFT_STRING,	0x50)
	ENTRY(altDropAnim,				alt_drop_anim,			WFT_STRING,	0x54)
	ENTRY(adsFireAnim,				ads_fire_anim,			WFT_STRING,	0x58)
	ENTRY(adsLastShotAnim,			ads_last_shot_anim,		WFT_STRING,	0x5c)
	ENTRY(adsRechamberAnim,			ads_rechamber_anim,		WFT_STRING,	0x60)
	ENTRY(adsUpAnim,				ads_up_anim,			WFT_STRING,	0x64)
	ENTRY(adsDownAnim,				ads_down_anim,			WFT_STRING,	0x68)

	ENTRY(script,					script,					WFT_STRING,	0x410)

	ENTRY(weaponType,				weapon_type,			WFT_WEAPON_TYPE, 0x70)
	ENTRY(weaponClass,				weapon_class,			WFT_WEAPON_CLASS, 0x74)
	ENTRY(weaponSlot,				weapon_slot,			WFT_WEAPON_SLOT, 0x78)
	ENTRY(slotStackable,			slot_stackable,			WFT_BOOL, 0x7c)

	ENTRY(viewFlashEffect,			view_flash_effect,		WFT_STRING,	0x84)
	ENTRY(worldFlashEffect,			world_flash_effect,		WFT_STRING,	0x88)

	ENTRY(pickupSound,				pickup_sound,			WFT_STRING,	0x8c)
	ENTRY(ammoPickupSound,			ammo_pickup_sound,		WFT_STRING,	0x90)
	ENTRY(projectileSound,			projectile_sound,		WFT_STRING,	0x94)
	ENTRY(pullbackSound,			pullback_sound,			WFT_STRING,	0x98)
	ENTRY(fireSound,				fire_sound,				WFT_STRING,	0x9c)
	ENTRY(loopFireSound,			loop_fire_sound,		WFT_STRING,	0xa0)
	ENTRY(stopFireSound,			stop_fire_sound,		WFT_STRING,	0xa4)
	ENTRY(fireEchoSound,			fire_echo_sound,		WFT_STRING,	0xa8)
	ENTRY(lastShotSound,			last_shot_sound,		WFT_STRING,	0xac)
	ENTRY(rechamberSound,			rechamber_sound,		WFT_STRING,	0xb0)
	ENTRY(reloadSound,				reload_sound,			WFT_STRING,	0xb4)
	ENTRY(reloadEmptySound,			reload_empty_sound,		WFT_STRING,	0xb8)
	ENTRY(reloadStartSound,			reload_start_sound,		WFT_STRING,	0xbc)
	ENTRY(reloadEndSound,			reload_end_sound,		WFT_STRING,	0xc0)
	ENTRY(raiseSound,				raise_sound,			WFT_STRING,	0xc4)
	ENTRY(altSwitchSound,			alt_switch_sound,		WFT_STRING,	0xc8)
	ENTRY(putAwaySound,				put_away_sound,			WFT_STRING,	0xcc)
	ENTRY(noteTrackSoundA,			note_track_sound_a,		WFT_STRING,	0xd0)
	ENTRY(noteTrackSoundB,			note_track_sound_b,		WFT_STRING,	0xd4)
	ENTRY(noteTrackSoundC,			note_track_sound_c,		WFT_STRING,	0xd8)
	ENTRY(noteTrackSoundD,			note_track_sound_d,		WFT_STRING,	0xdc)

	ENTRY(shellEjectEffect,			shell_eject_effect,		WFT_STRING,	0xe0)
	ENTRY(lastShotEjectEffect,		last_shot_eject_effect,	WFT_STRING,	0xe4)

	ENTRY(reticleCenter,			reticle_center,			WFT_STRING, 0xe8)
	ENTRY(reticleSide,				reticle_side,			WFT_STRING, 0xec)
	ENTRY(reticleCenterSize,		reticle_center_size,	WFT_INT, 0xf0)
	ENTRY(reticleSideSize,			reticle_side_size,		WFT_INT, 0xf4)
	ENTRY(reticleMinOfs,			reticle_min_ofs,		WFT_INT, 0xf8)

	ENTRY(standMoveF,				stand_move_f,			WFT_FLOAT, 0xfc)
	ENTRY(standMoveR,				stand_move_r,			WFT_FLOAT, 0x100)
	ENTRY(standMoveU,				stand_move_u,			WFT_FLOAT, 0x104)
	ENTRY(standRotP,				stand_rot_p,			WFT_FLOAT, 0x108)
	ENTRY(standRotY,				stand_rot_y,			WFT_FLOAT, 0x10c)
	ENTRY(standRotR,				stand_rot_r,			WFT_FLOAT, 0x110)
	ENTRY(duckedOfSf,				ducked_of_sf,			WFT_FLOAT, 0x114)
	ENTRY(duckedOfSr,				ducked_of_sr,			WFT_FLOAT, 0x118)
	ENTRY(duckedOfSu,				ducked_of_su,			WFT_FLOAT, 0x11c)
	ENTRY(duckedMoveF,				ducked_move_f,			WFT_FLOAT, 0x120)
	ENTRY(duckedMoveR,				ducked_move_r,			WFT_FLOAT, 0x124)
	ENTRY(duckedMoveU,				ducked_move_u,			WFT_FLOAT, 0x128)
	ENTRY(duckedRotP,				ducked_rot_p,			WFT_FLOAT, 0x12c)
	ENTRY(duckedRotY,				ducked_rot_y,			WFT_FLOAT, 0x130)
	ENTRY(duckedRotR,				ducked_rot_r,			WFT_FLOAT, 0x134)
	ENTRY(proneOfSf,				prone_of_sf,			WFT_FLOAT, 0x138)
	ENTRY(proneOfSr,				prone_of_sr,			WFT_FLOAT, 0x13c)
	ENTRY(proneOfSu,				prone_of_su,			WFT_FLOAT, 0x140)
	ENTRY(proneMoveF,				prone_move_f,			WFT_FLOAT, 0x144)
	ENTRY(proneMoveR,				prone_move_r,			WFT_FLOAT, 0x148)
	ENTRY(proneMoveU,				prone_move_u,			WFT_FLOAT, 0x14c)
	ENTRY(proneRotP,				prone_rot_p,			WFT_FLOAT, 0x150)
	ENTRY(proneRotY,				prone_rot_y,			WFT_FLOAT, 0x154)
	ENTRY(proneRotR,				prone_rot_r,			WFT_FLOAT, 0x158)
	ENTRY(posMoveRate,				pos_move_rate,			WFT_FLOAT, 0x15c)
	ENTRY(posProneMoveRate,			pos_prone_move_rate,	WFT_FLOAT, 0x160)
	ENTRY(standMoveMinSpeed,		stand_move_min_speed,	WFT_FLOAT, 0x164)
	ENTRY(duckedMoveMinSpeed,		ducked_move_min_speed,	WFT_FLOAT, 0x168)
	ENTRY(proneMoveMinSpeed,		prone_move_min_speed,	WFT_FLOAT, 0x16c)
	ENTRY(posRotRate,				pos_rot_rate,			WFT_FLOAT, 0x170)
	ENTRY(posProneRotRate,			pos_prone_rot_rate,		WFT_FLOAT, 0x174)
	ENTRY(standRotMinSpeed,			stand_rot_min_speed,	WFT_FLOAT, 0x178)
	ENTRY(duckedRotMinSpeed,		ducked_rot_min_speed,	WFT_FLOAT, 0x17c)
	ENTRY(proneRotMinSpeed,			prone_rot_min_speed,	WFT_FLOAT, 0x180)

	ENTRY(radiantName,				radiant_name,			WFT_STRING, 0x184)
	ENTRY(worldModel,				world_model,			WFT_STRING, 0x188)
	ENTRY(hudIcon,					hud_icon,				WFT_STRING, 0x18c)
	ENTRY(modeIcon,					mode_icon,				WFT_STRING, 0x190)
	ENTRY(ammoIcon,					ammo_icon,				WFT_STRING, 0x194)

	ENTRY(startAmmo,				start_ammo,				WFT_INT, 0x198)
	ENTRY(ammoName,					ammo_name,				WFT_STRING, 0x19c)
	ENTRY(clipName,					clip_name,				WFT_STRING, 0x1a4)
	ENTRY(maxAmmo,					max_ammo,				WFT_INT, 0x1ac)
	ENTRY(clipSize,					clip_size,				WFT_INT, 0x1b0)
	ENTRY(sharedAmmoCapName,		shared_ammo_cap_name,	WFT_STRING, 0x1b4)
	ENTRY(sharedAmmoCap,			shared_ammo_cap,		WFT_INT, 0x1bc)
	ENTRY(damage,					damage,					WFT_INT, 0x1c0)
	ENTRY(meleeDamage,				melee_damage,			WFT_INT, 0x1c4)

	ENTRY(fireDelay,				fire_delay,				WFT_TIME, 0x1cc)
	ENTRY(meleeDelay,				melee_delay,			WFT_TIME, 0x1d0)
	ENTRY(fireTime,					fire_time,				WFT_TIME, 0x1d4)
	ENTRY(rechamberTime,			rechamber_time,			WFT_TIME, 0x1d8)
	ENTRY(rechamberBoltTime,		rechamber_bolt_time,	WFT_TIME, 0x1dc)
	ENTRY(holdFireTime,				hold_fire_time,			WFT_TIME, 0x1e0)
	ENTRY(meleeTime,				melee_time,				WFT_TIME, 0x1e4)
	ENTRY(reloadTime,				reload_time,			WFT_TIME, 0x1e8)
	ENTRY(reloadEmptyTime,			reload_empty_time,		WFT_TIME, 0x1ec)
	ENTRY(reloadAddTime,			reload_add_time,		WFT_TIME, 0x1f0)
	ENTRY(reloadStartTime,			reload_start_time,		WFT_TIME, 0x1f4)
	ENTRY(reloadStartAddTime,		reload_start_add_time,	WFT_TIME, 0x1f8)
	ENTRY(reloadEndTime,			reload_end_time,		WFT_TIME, 0x1fc)
	ENTRY(dropTime,					drop_time,				WFT_TIME, 0x200)
	ENTRY(raiseTime,				raise_time,				WFT_TIME, 0x204)
	ENTRY(altDropTime,				alt_drop_time,			WFT_TIME, 0x208)
	ENTRY(altRaiseTime,				alt_raise_time,			WFT_TIME, 0x20c)
	ENTRY(fuseTime,					fuse_time,				WFT_TIME, 0x210)

	ENTRY(moveSpeedScale,			move_speed_scale,		WFT_FLOAT, 0x214)
	ENTRY(idleCrouchFactor,			idle_crouch_factor,		WFT_FLOAT, 0x278)
	ENTRY(idleProneFactor,			idle_prone_factor,		WFT_FLOAT, 0x27c)

	ENTRY(gunMaxPitch,				gun_max_pitch,			WFT_FLOAT, 0x280)
	ENTRY(gunMaxYaw,				gun_max_yaw,			WFT_FLOAT, 0x284)
	ENTRY(swayMaxAngle,				sway_max_angle,			WFT_FLOAT, 0x288)
	ENTRY(swayLerpSpeed,			sway_lerp_speed,		WFT_FLOAT, 0x28c)
	ENTRY(swayPitchScale,			sway_pitch_scale,		WFT_FLOAT, 0x290)
	ENTRY(swayYawScale,				sway_yaw_scale,			WFT_FLOAT, 0x294)
	ENTRY(swayHorizScale,			sway_horiz_scale,		WFT_FLOAT, 0x298)
	ENTRY(swayVertScale,			sway_vert_scale,		WFT_FLOAT, 0x29c)
	ENTRY(swayShellshockScale,		sway_shellshock_scale,	WFT_FLOAT, 0x2a0)
	ENTRY(adsSwayMaxAngle,			ads_sway_max_angle,		WFT_FLOAT, 0x2a4)
	ENTRY(adsSwayLerpSpeed,			ads_sway_lerp_speed,	WFT_FLOAT, 0x2a8)
	ENTRY(adsSwayPitchScale,		ads_sway_pitch_scale,	WFT_FLOAT, 0x2ac)
	ENTRY(adsSwayYawScale,			ads_sway_yaw_scale,		WFT_FLOAT, 0x2b0)
	ENTRY(adsSwayHorizScale,		ads_sway_horiz_scale,	WFT_FLOAT, 0x2b4)
	ENTRY(adsSwayVertScale,			ads_sway_vert_scale,	WFT_FLOAT, 0x2b8)

	ENTRY(rifleBullet,				rifle_bullet,			WFT_BOOL, 0x2c0)
	ENTRY(twoHanded,				two_handed,				WFT_BOOL, 0x2bc)
	ENTRY(semiAuto,					semi_auto,				WFT_BOOL, 0x2c4)
	ENTRY(boltAction,				bolt_action,			WFT_BOOL, 0x2c8)
	ENTRY(aimDownSight,				aim_down_sight,			WFT_BOOL, 0x2cc)

	#ifdef CODVERSION1_5
	ENTRY(rechamberWhileAiming,		rechamber_while_aiming,	WFT_BOOL, 0x2d0)
	ENTRY(adsViewErrorMin,			ads_view_error_min,		WFT_FLOAT, 0x2d4)
	ENTRY(adsViewErrorMax,			ads_view_error_max,		WFT_FLOAT, 0x2d8)
	#endif

	ENTRY(clipOnly,					clip_only,				WFT_BOOL, 0x2d4)
	ENTRY(cookOffHold,				cook_off_hold,			WFT_BOOL, 0x2d0)
	ENTRY(wideListIcon,				wide_list_icon,			WFT_BOOL, 0x2d8)
	ENTRY(adsFire,					ads_fire,				WFT_BOOL, 0x2dc)

	ENTRY(killIcon,					kill_icon,				WFT_STRING, 0x2e0)
	ENTRY(wideKillIcon,				wide_kill_icon,			WFT_BOOL,	0x2e4)
	ENTRY(noPartialReload,			no_partial_reload,		WFT_BOOL, 0x2e8)
	ENTRY(segmentedReload,			segmented_reload,		WFT_BOOL, 0x2ec)
	ENTRY(reloadAmmoAdd,			reload_ammo_add,		WFT_INT, 0x2f0)
	ENTRY(reloadStartAdd,			reload_start_add,		WFT_INT, 0x2f4)
	ENTRY(altWeapon,				alt_weapon,				WFT_STRING, 0x2f8)

	ENTRY(dropAmmoMin,				drop_ammo_min,			WFT_INT, 0x300)
	ENTRY(dropAmmoMax,				drop_ammo_max,			WFT_INT, 0x304)

	ENTRY(explosionRadius,			explosion_radius,		WFT_INT, 0x308)
	ENTRY(explosionInnerDamage,		explosion_inner_damage, WFT_INT, 0x30c)
	ENTRY(explosionOuterDamage,		explosion_outer_damage, WFT_INT, 0x310)

	ENTRY(projectileSpeed,			projectile_speed,		WFT_INT, 0x314)
	ENTRY(projectileSpeedUp,		projectile_speed_up,	WFT_INT, 0x318)
	ENTRY(projectileModel,			projectile_model,		WFT_STRING, 0x31c)
	ENTRY(projExplosionType,		projectile_explosion_type, WFT_PROJECTILE_TYPE, 0x320)
	ENTRY(projExplosionEffect,		projectile_explosion_effect, WFT_STRING, 0x324)
	ENTRY(projExplosionSound,		projectile_explosion_sound, WFT_STRING, 0x328)
	ENTRY(projImpactExplode,		projectile_impact_explode, WFT_BOOL, 0x32c)
	ENTRY(projTrailEffect,			projectile_trail_effect, WFT_STRING, 0x330)
	ENTRY(projectileDLight,			projectile_dlight,		WFT_INT, 0x334)
	ENTRY(projectileRed,			projectile_red,			WFT_FLOAT, 0x338)
	ENTRY(projectileGreen,			projectile_green,		WFT_FLOAT, 0x33c)
	ENTRY(projectileBlue,			projectile_blue,		WFT_FLOAT, 0x340)

	ENTRY(adsTransInTime,			ads_trans_in_time,		WFT_TIME, 0x268)
	ENTRY(adsTransOutTime,			ads_trans_out_time,		WFT_TIME, 0x26c)
	ENTRY(adsIdleAmount,			ads_idle_amount,		WFT_FLOAT, 0x270)

	ENTRY(adsZoomFov,				ads_zoom_fov,			WFT_FLOAT, 0x218)
	ENTRY(adsZoomInFrac,			ads_zoom_in_frac,		WFT_FLOAT, 0x21c)
	ENTRY(adsZoomOutFrac,			ads_zoom_out_frac,		WFT_FLOAT, 0x220)
	ENTRY(adsOverlayShader,			ads_overlay_shader,		WFT_STRING, 0x224)
	ENTRY(adsOverlayReticle,		ads_overlay_reticle,	WFT_OVERLAY_RETICLE, 0x228)
	ENTRY(adsOverlayWidth,			ads_overlay_width,		WFT_FLOAT, 0x22c)
	ENTRY(adsOverlayHeight,			ads_overlay_height,		WFT_FLOAT, 0x230)
	ENTRY(adsBobFactor,				ads_bob_factor,			WFT_FLOAT, 0x234)
	ENTRY(adsViewBobMult,			ads_view_bob_mult,		WFT_FLOAT, 0x238)

	ENTRY(adsAimPitch,				ads_aim_pitch,			WFT_FLOAT, 0x344)
	ENTRY(adsCrosshairInFrac,		ads_crosshair_in_frac,	WFT_FLOAT, 0x348)
	ENTRY(adsCrosshairOutFrac,		ads_crosshair_out_frac, WFT_FLOAT, 0x34c)
	
	ENTRY(adsReloadTransTime,		ads_reload_trans_time,	WFT_TIME, 0x3d4)
	ENTRY(adsTransBlendTime,		ads_trans_blend_time,	WFT_TIME, 0x3d8)

	ENTRY(adsGunKickPitchMin,		ads_gun_kick_pitch_min, WFT_FLOAT, 0x350)
	ENTRY(adsGunKickPitchMax,		ads_gun_kick_pitch_max, WFT_FLOAT, 0x354)
	ENTRY(adsGunKickYawMin,			ads_gun_kick_yaw_min,	WFT_FLOAT, 0x358)
	ENTRY(adsGunKickYawMax,			ads_gun_kick_yaw_max,	WFT_FLOAT, 0x35c)
	ENTRY(adsGunKickAccel,			ads_gun_kick_accel,		WFT_FLOAT, 0x360)
	ENTRY(adsGunKickSpeedMax,		ads_gun_kick_speed_max, WFT_FLOAT, 0x364)
	ENTRY(adsGunKickSpeedDecay,		ads_gun_kick_speed_decay, WFT_FLOAT, 0x368)
	ENTRY(adsGunKickStaticDecay,	ads_gun_kick_static_decay, WFT_FLOAT, 0x36c)
	ENTRY(adsViewKickPitchMin,		ads_view_kick_pitch_min, WFT_FLOAT, 0x370)
	ENTRY(adsViewKickPitchMax,		ads_view_kick_pitch_max, WFT_FLOAT, 0x374)
	ENTRY(adsViewKickYawMin,		ads_view_kick_yaw_min,	WFT_FLOAT, 0x378)
	ENTRY(adsViewKickYawMax,		ads_view_kick_yaw_max,	WFT_FLOAT, 0x37c)
	ENTRY(adsViewKickCenterSpeed,	ads_view_kick_center_speed, WFT_FLOAT, 0x380)

	ENTRY(adsSpread,				ads_spread,				WFT_FLOAT, 0x38c)

	ENTRY(hipSpreadStandMin,		hip_spread_stand_min,	WFT_FLOAT, 0x23c)
	ENTRY(hipSpreadDuckedMin,		hip_spread_ducked_min,	WFT_FLOAT, 0x240)
	ENTRY(hipSpreadProneMin,		hip_spread_prone_min,	WFT_FLOAT, 0x244)
	ENTRY(hipSpreadMax,				hip_spread_max,			WFT_FLOAT, 0x248)
	ENTRY(hipSpreadDecayRate,		hip_spread_decay_rate,	WFT_FLOAT, 0x24c)
	ENTRY(hipSpreadFireAdd,			hip_spread_fire_add,	WFT_FLOAT, 0x250)
	ENTRY(hipSpreadTurnAdd,			hip_spread_turn_add,	WFT_FLOAT, 0x254)
	ENTRY(hipSpreadMoveAdd,			hip_spread_move_add,	WFT_FLOAT, 0x258)
	ENTRY(hipSpreadDuckedDecay,		hip_spread_ducked_decay, WFT_FLOAT, 0x25c)
	ENTRY(hipSpreadProneDecay,		hip_spread_prone_decay, WFT_FLOAT, 0x260)
	ENTRY(hipReticleSidePos,		hip_reticle_side_pos,	WFT_FLOAT, 0x264)

	ENTRY(hipIdleAmount,			hip_idle_amount,		WFT_FLOAT, 0x274)

	ENTRY(hipGunKickPitchMin,		hip_gun_kick_pitch_min, WFT_FLOAT, 0x390)
	ENTRY(hipGunKickPitchMax,		hip_gun_kick_pitch_max, WFT_FLOAT, 0x394)
	ENTRY(hipGunKickYawMin,			hip_gun_kick_yaw_min,	WFT_FLOAT, 0x398)
	ENTRY(hipGunKickYawMax,			hip_gun_kick_yaw_max,	WFT_FLOAT, 0x39c)
	ENTRY(hipGunKickAccel,			hip_gun_kick_accel,		WFT_FLOAT, 0x3a0)
	ENTRY(hipGunKickSpeedMax,		hip_gun_kick_speed_max, WFT_FLOAT, 0x3a4)
	ENTRY(hipGunKickSpeedDecay,		hip_gun_kick_speed_decay, WFT_FLOAT, 0x3a8)
	ENTRY(hipGunKickSpeedStatic,	hip_gun_kick_speed_static, WFT_FLOAT, 0x3ac)
	ENTRY(hipViewKickPitchMin,		hip_view_kick_pitch_min, WFT_FLOAT, 0x3b0)
	ENTRY(hipViewKickPitchMax,		hip_view_kick_pitch_max, WFT_FLOAT, 0x3b4)
	ENTRY(hipViewKickYawMin,		hip_view_kick_yaw_min,	WFT_FLOAT, 0x3b8)
	ENTRY(hipViewKickYawMax,		hip_view_kick_yaw_max,	WFT_FLOAT, 0x3bc)
	ENTRY(hipViewKickCenterSpeed,	hip_view_kick_center_speed, WFT_FLOAT, 0x3c0)

	ENTRY(leftArc,					left_arc,				WFT_FLOAT, 0x3dc)
	ENTRY(rightArc,					right_arc,				WFT_FLOAT, 0x3e0)
	ENTRY(topArc,					top_arc,				WFT_FLOAT, 0x3e4)
	ENTRY(bottomArc,				bottom_arc,				WFT_FLOAT, 0x3e8)
	ENTRY(accuracy,					accuracy,				WFT_FLOAT, 0x3ec)
	ENTRY(vertTurnSpeed,			vert_turn_speed,		WFT_FLOAT, 0x3f0)
	ENTRY(horTurnSpeed,				hor_turn_speed,			WFT_FLOAT, 0x3f4)
	ENTRY(convergenceTime,			convergence_time,		WFT_FLOAT, 0x3f8)
	ENTRY(maxRange,					max_range,				WFT_FLOAT, 0x3fc)
	ENTRY(animHorRotateInc,			anim_hor_rotate_inc,	WFT_FLOAT, 0x400)
	ENTRY(playerPositionDist,		player_position_dist,	WFT_FLOAT, 0x404)

	ENTRY(stance,					stance,					WFT_WEAPON_STANCE, 0x80)

	ENTRY(useHintString,			use_hint_string,		WFT_STRING, 0x408)

	#ifdef CODVERSION1_5
	ENTRY(horizViewJitter,			horiz_view_jitter,		WFT_FLOAT, 0x41c)
	ENTRY(vertViewJitter,			vert_view_jitter,		WFT_FLOAT, 0x420)
	#endif

	ENTRY(aiEffectiveRange,			ai_effective_range,		WFT_FLOAT, 0x3cc)
	ENTRY(aiMissRange,				ai_miss_range,			WFT_FLOAT, 0x3d0)

	#undef ENTRY
	{ NULL }
};
int config_strings_size = ARRAY_SIZE(config_strings);
//

struct weapon **bg_weapons = NULL;
int bg_num_weapons;
intptr_t *bg_weapon_alloc_ptr;

void bg_allocate_weapon_string(char **loc, char *str)
{
	int len;
	char *out;

	if (*str == '\0') {
		*loc = (char *) bg_weapon_alloc_ptr;
	} else {
		len = strlen(str);

		out = trap_hunk_alloc_low_align_internal(len + 1, 1);
		strcpy(out, str);
		*loc = out;
	}
}

#define STRP(c, x, s) c((intptr_t) &x->number + s->offset)

struct weapon *bg_allocate_weapon_strings(int num, 
	struct weapon_config_string *cfg_strings, int count) {
	struct weapon *weapon;
	struct weapon_config_string *cfg_str;
	int i;

	weapon = trap_hunk_alloc_low_internal(sizeof(struct weapon));
	bg_weapons[num] = weapon;
	weapon->number = num;

	bg_allocate_weapon_string(&weapon->name, "");

	for (i = 0, cfg_str = cfg_strings; i < count; i++, cfg_str++) {
		if (cfg_str->type == WFT_STRING)
			bg_allocate_weapon_string(STRP((char **), weapon, cfg_str), "");
	}

	return weapon;
}

bool parse_weapon_file_struct_data(void *data, char *str, int type)
{
	int i;
	struct weapon *weapon = (struct weapon *) data;

	switch (type) {
		case WFT_WEAPON_TYPE:
			for (i = 0; i < weapon_type_size; i++) {
				if (!strcasecmp(str, weapon_type_str[i])) {
					weapon->weapon_type = i;
					break;
				}
			}

			if (i == weapon_type_size)
				g_error("Unknown weapon type '%s' in '%s'", str, weapon->name);

			break;
		case WFT_WEAPON_CLASS:
			for (i = 0; i < weapon_class_size; i++) {
				if (!strcasecmp(str, weapon_class_str[i])) {
					weapon->weapon_class = i;
					break;
				}
			}

			if (i == weapon_class_size)
				g_error("Unknown weapon class '%s' in '%s", str, weapon->name);

			break;
		case WFT_OVERLAY_RETICLE:
			for (i = 0; i < overlay_reticle_size; i++) {
				if (!strcasecmp(str, overlay_reticle_str[i])) {
					weapon->ads_overlay_reticle = i;
					break;
				}
			}

			if (i == overlay_reticle_size)
				g_error("Unknown weapon overlay reticle '%s' in '%s'", str,
						weapon->name);

			break;
		case WFT_WEAPON_SLOT:
			for (i = 0; i < weapon_slot_size; i++) {
				if (!strcasecmp(str, weapon_slot_str[i])) {
					weapon->weapon_slot = i;
					break;
				}
			}

			if (i == weapon_slot_size)
				g_error("Unknown weapon slot '%s' in '%s", str, weapon->name);

			break;
		case WFT_WEAPON_STANCE:
			for (i = 0; i < weapon_stance_size; i++) {
				if (!strcasecmp(str, weapon_stance_str[i])) {
					weapon->stance = i;
					break;
				}
			}

			if (i == weapon_stance_size)
				g_error("Unknown weapon stance '%s' in '%s'", str, 
						weapon->name);

			break;
		case WFT_PROJECTILE_TYPE:
			for (i = 0; i < projectile_type_size; i++) {
				if (!strcasecmp(str, projectile_type_str[i])) {
					weapon->projectile_explosion_type = i;
					break;
				}
			}

			if (i == projectile_type_size)
				g_error("Unknown projectile explosion '%s' in '%s", str,
						weapon->name);

			break;
		default:
			g_error("Bad field type %i in %s", type, weapon->name);
			return false;
	}

	return true;
}

bool parse_config_string_to_struct(struct weapon *weapon,
								   struct weapon_config_string *cfg_strings,
								   int count,
								   char *data,
								   int max_types,
								   int (*external_func)(void *, char *, int))
{
	int i;
	struct weapon_config_string *cfg_str;
	char *val, *str;

	for (i = 0, cfg_str = cfg_strings; i < count; i++, cfg_str++) {
		val = info_value_for_key(data, cfg_str->string);
		if (!val[0])
			continue;

		switch (cfg_str->type) {
			case WFT_STRING:
				bg_allocate_weapon_string(STRP((char **), weapon, cfg_str), 
										  val);
				break;

			// 1-3 aren't used
			case 1:
				str = STRP((char *), weapon, cfg_str);
				strncpy(str, val, 1023);
				str[1023] = '\0';
				break;
			case 2:
				str = STRP((char *), weapon, cfg_str);
				strncpy(str, val, 63);
				str[63] = '\0';
				break;
			case 3:
				str = STRP((char *), weapon, cfg_str);
				strncpy(str, val, 255);
				str[255] = '\0';
				break;
			// unused

			case WFT_INT:
				STRP(*(int *), weapon, cfg_str) = atoi(val);
				break;
			case WFT_BOOL:
				STRP(*(bool *), weapon, cfg_str) = (atoi(val) != 0);
				break;
			case WFT_FLOAT:
				STRP(*(float *), weapon, cfg_str) = atof(val);
				break;
			case WFT_TIME:
				STRP(*(float *), weapon, cfg_str) = round(atof(val) * 1000.0);
				break;

			default:
				if (max_types < 1 || cfg_str->type > max_types)
					g_error("Bad field type: %d", cfg_str->type);

				if (!(external_func) (weapon, val, cfg_str->type))
					return false;
		}
	}

	if (i == count)
		return true;
	else
		return false;
}

void bg_parse_weapon_files(char **list, int num)
{
	int header_len, i, file_len;
	struct weapon *weapon;
	char filename[MAX_QPATH];
	filehandle f;
	char data[BIG_INFO_STRING];

	header_len = strlen(WEAPONFILE_HEADER);

	bg_weapon_alloc_ptr = trap_hunk_alloc_low_align_internal(1, 1);
	*bg_weapon_alloc_ptr = 0;

	weapon = bg_allocate_weapon_strings(0, config_strings, config_strings_size);
	bg_allocate_weapon_string(&weapon->name, "none");

	bg_num_weapons = 0;

	for (i = 0; i < num; i++) {
		bg_num_weapons++;

		weapon = bg_allocate_weapon_strings(bg_num_weapons, config_strings,
											config_strings_size);

		snprintf(filename, sizeof(filename), "%s/", BG_WEAPONS_FOLDER);
		strcat(filename, list[i]);

		g_dprintf("parsing weapon file '%s'\n", filename);

		file_len = trap_fs_fopen_file(filename, &f, FS_READ);
		if (file_len < 1)
			g_error("Could not load weapon file '%s'", filename);

		trap_fs_read(data, header_len, f);
		data[header_len] = '\0';

		if (strncmp(data, WEAPONFILE_HEADER, header_len))
			g_error("'%s' does not appear to be a weapon file", filename);

		if (file_len > MAX_WEAPONFILE_SIZE)
			g_error("'%s' is too long of a weapon file to parse", filename);

		memset(data, 0, sizeof(data));
		trap_fs_read(data, file_len - header_len, f);
		data[file_len - header_len] = '\0';

		trap_fs_fclose_file(f);

		if (!info_validate(data))
			g_error("'%s' is not a valid weapon file", filename);

		bg_allocate_weapon_string(&weapon->name, list[i]);

		if (!parse_config_string_to_struct(weapon, config_strings,
										   config_strings_size,
										   data,
										   WFT_TYPE_MAX,
										   parse_weapon_file_struct_data)) {
			bg_weapons[bg_num_weapons] = NULL;
			bg_num_weapons--;
		}
	}
}

void bg_set_ads_trans_times(void)
{
	int i;
	struct weapon *weapon;
	
	for (i = 1; i <= bg_num_weapons; i++) {
		weapon = bg_weapons[i];

		if (weapon->ads_trans_in_time < 1)
			weapon->ads_trans_in_time_f = 0.00333333;
		else
			weapon->ads_trans_in_time_f = 1.0 / weapon->ads_trans_in_time;

		if (weapon->ads_trans_out_time < 1)
			weapon->ads_trans_out_time_f = 0.002;
		else
			weapon->ads_trans_out_time_f = 1.0 / weapon->ads_trans_out_time;
	}
}

//
// ammo types
//
static int num_ammo_types;
static char *ammo_types[MAX_WEAPONS];
static int ammo_max[MAX_WEAPONS];

int bg_get_num_ammo_types(void)
{
	return num_ammo_types;
}

int bg_get_ammo_type_for_name(const char *name)
{
	int i;

	for (i = 0; i < num_ammo_types; i++ ) {
		if (!q_stricmp(ammo_types[i], name))
			return i;
	}

	g_dprintf("Couldn't find ammo type '%s'\n", name);
	return 0;
}

static void bg_setup_ammo_indexes(void)
{
	int i, j, k;
	struct weapon *weapon, *weapon2;

	for (i = 1; i < bg_num_weapons; i++) {
		weapon = bg_weapons[i];

		q_strlwr(weapon->ammo_name);

		for (j = 0; j < num_ammo_types; j++) {
			if (!q_stricmp(ammo_types[j], weapon->ammo_name)) {
				weapon->ammo_name_index = j;

				if (j && ammo_max[j] != weapon->max_ammo) {
					for (k = 1; k < i; k++) {
						weapon2 = bg_weapons[k];
						if (!q_stricmp(ammo_types[j], weapon2->ammo_name) &&
							weapon2->max_ammo == ammo_max[j]) {
							g_error("Max ammo mismatch for '%s' ammo: " \
									"'%s' set to %d, but '%s' already " \
									"set it to %d",
									weapon->ammo_name, weapon->name, 
									weapon->max_ammo,weapon2->name, 
									weapon2->max_ammo);
						}
					}
				}
			}
		}

		if (j == num_ammo_types) {
			ammo_types[j] = weapon->ammo_name;
			ammo_max[j] = weapon->max_ammo;
			weapon->ammo_name_index = j;
			num_ammo_types++;
		}

		weapon++;
	}
}

//
// shared ammo
//
static int num_shared_ammo_caps;
static char *shared_ammo_cap_names[MAX_WEAPONS];
static int shared_ammo_caps[MAX_WEAPONS];

static void bg_setup_shared_ammo_indexes(void)
{
	int i, j, k;
	struct weapon *weapon, *weapon2;

	for (i = 1; i < bg_num_weapons; i++) {
		weapon = bg_weapons[i];

		weapon->shared_ammo_cap_index = -1;
		if (!weapon->shared_ammo_cap_name[0]) {
			weapon++;
			continue;
		}

		q_strlwr(weapon->shared_ammo_cap_name);

		for (j = 0; j < num_shared_ammo_caps; j++) {
			if (!q_stricmp(shared_ammo_cap_names[j], 
						   weapon->shared_ammo_cap_name)) {
				weapon->shared_ammo_cap_index = j;

				if (j && shared_ammo_caps[j] != weapon->shared_ammo_cap) {
					for (k = 1; k < i; k++) {
						weapon2 = bg_weapons[k];
						if (!q_stricmp(shared_ammo_cap_names[j], 
									   weapon2->shared_ammo_cap_name) &&
							weapon2->shared_ammo_cap == shared_ammo_caps[j]) {
							g_error("Shared ammo mismatch for '%s' ammo: " \
									"'%s' set to %d, but '%s' already " \
									"set it to %d",
									weapon->shared_ammo_cap_name, weapon->name,
									weapon->shared_ammo_cap, weapon2->name,
									weapon2->shared_ammo_cap);
						}
					}
				}
			}
		}

		if (j == num_shared_ammo_caps) {
			shared_ammo_cap_names[j] = weapon->shared_ammo_cap_name;
			shared_ammo_caps[j] = weapon->shared_ammo_cap;
			weapon->shared_ammo_cap_index = j;
			num_shared_ammo_caps++;
		}

		weapon++;
	}
}

//
// clips
//
static int num_clip_types;
static char *clip_types[MAX_WEAPONS];
static int clip_max[MAX_WEAPONS];

int bg_get_num_ammo_clips(void)
{
	return num_clip_types;
}

int bg_get_ammo_clip_for_name(const char *name)
{
	int i;

	for (i = 0; i < num_clip_types; i++) {
		if (!q_stricmp(clip_types[i], name))
			return i;
	}

	g_dprintf("Couldn't find ammo clip '%s'\n", name);
	return 0;
}

static void bg_setup_clip_indexes(void)
{
	int i, j, k;
	struct weapon *weapon, *weapon2;

	for (i = 1; i < bg_num_weapons; i++) {
		weapon = bg_weapons[i];

		q_strlwr(weapon->clip_name);

		for (j = 0; j < num_clip_types; j++) {
			if (!q_stricmp(clip_types[j], weapon->clip_name)) {
				weapon->clip_name_index = j;

				if (j && clip_max[j] != weapon->clip_size) {
					for (k = 1; k < i; k++) {
						weapon2 = bg_weapons[k];
						if (!q_stricmp(clip_types[j], weapon2->clip_name) &&
							weapon2->clip_size == clip_max[j]) {
							g_error("Clip size mismatch for '%s' ammo: " \
									"'%s' set to %d, but '%s' already " \
									"set it to %d",
									weapon->clip_name, weapon->name,
									weapon->clip_size, weapon2->name,
									weapon2->clip_size);
						}
					}
				}
			}
		}

		if (j == num_clip_types) {
			clip_types[j] = weapon->clip_name;
			clip_max[j] = weapon->clip_size;
			weapon->clip_name_index = j;
			num_clip_types++;
		}

		weapon++;
	}
}

void bg_fill_in_weapon_items(void)
{
	int i, j;
	struct gitem *item;
	struct weapon *weapon;

	// drop weapons into the 64 slots first available
	for (i = 1; i <= bg_num_weapons; i++) {
		item = &bg_item_list[i];
		weapon = bg_weapons[i];

		item->class_name = weapon->radiant_name;
		item->pickup_sound = weapon->pickup_sound;
		item->world_model[0] = weapon->world_model;
		item->world_model[1] = NULL; // not used
		item->hud_icon = weapon->hud_icon;
		item->ammo_icon = weapon->ammo_icon;
		item->display_name = weapon->display_name;
		item->quantity = weapon->start_ammo;
		item->type = IT_WEAPON;
		item->tag = i;
		item->ammo_name_index = weapon->ammo_name_index;
		item->clip_name_index = weapon->clip_name_index;
	}

	// update ammo items
	for (; i < 69; i++) {
		item = &bg_item_list[i];

		if (item->type == IT_AMMO) {
			for (j = 1; j <= bg_num_weapons; j++) {
				weapon = bg_weapons[j];
				if (!q_stricmpn(item->display_name, weapon->name,
								strlen(weapon->name))) {
					item->tag = j;
					item->ammo_name_index = weapon->ammo_name_index;
					item->clip_name_index = weapon->clip_name_index;
					break;
				}
			}

			if (item->tag == -1) {
				g_printf("WARNING: Could not find weapon for ammo item '%s'\n",
						 item->display_name);

				weapon = bg_weapons[1];
				item->tag = 1;
				item->ammo_name_index = weapon->ammo_name_index;
				item->clip_name_index = weapon->clip_name_index;
			}
		}
	}
}

void bg_setup_alt_weapon_indexes(void)
{
	int i, j;
	struct weapon *weapon, *weapon2, *oldweapon;

	// not sure why this is but it is
	for (i = 1; i < bg_num_weapons; i++)
		bg_weapons[i]->alt_weapon_index = 0;

	for (i = 1; i < bg_num_weapons; i++) {
		oldweapon = bg_weapons[i];

		if (!oldweapon->alt_weapon_index && oldweapon->alt_weapon[0] != '\0') {
			weapon = oldweapon;

			while (weapon->alt_weapon_index == 0) {
				for (j = 1; j <= bg_num_weapons; j++) {
					weapon2 = bg_weapons[j];

					if (!q_stricmp(weapon->alt_weapon, weapon2->name)) {
						weapon->alt_weapon_index = j;
						if (weapon->weapon_slot != weapon2->weapon_slot) {
							g_error("Weapon '%s' does not have the same weapon " \
									"slot setting as its alt weapon '%s'",
									weapon->name, weapon2->name);
						}

						if (weapon->slot_stackable != weapon2->slot_stackable) {
							g_error("Weapon '%s' does not have the same slot " \
									"stackable setting as its alt weapon '%s'",
									weapon->name, weapon2->name);
						}

						break;
					}
				}

				if (!weapon->alt_weapon_index) {
					g_error("Could not find alt weapon '%s' for weapon '%s'",
							oldweapon->alt_weapon, oldweapon->name);
				}

				weapon = bg_weapons[j];
			}

			if (oldweapon != weapon) {
				g_error("Weapon '%s' has bad alt weapon '%s'", oldweapon->name,
						oldweapon->alt_weapon);
			}
		}
	}
}

void bg_setup_weapon_hint_strings(void)
{
	struct weapon *weapon;
	int i;

	for (i = 1; i <= bg_num_weapons; i++) {
		weapon = bg_weapons[i];

		if (weapon->use_hint_string[0] != '\0' &&
			!g_get_hint_string_index(&weapon->hint_string_index,
									 weapon->use_hint_string)) {
			g_error("Too many different hint string values on weapons. Max " \
					"allowed is %d different strings", MAX_HINT_STRINGS);
		}
	}
}

static int compare_weapon_file_names(const void *a, const void *b)
{
	char *aa, *bb;

	aa = *(char **) a;
	bb = *(char **) b;

	return q_stricmp(aa, bb);
}

void bg_set_up_weapon_info(void)
{
	int num_weapons, i, len;
	char *weapons[MAX_WEAPONS];
	char *weapon;
	char file_list[4096];
	char config_string[BIG_INFO_STRING];
	bool already_allocated = false;

	bg_weapons = trap_get_weapon_info_memory(sizeof(*bg_weapons) * MAX_WEAPONS, 
											 &already_allocated);

	if (bg_weapons == NULL)
		g_error("Could not allocate weapon info array");

	// zero ammo types array
	memset(ammo_types, 0, sizeof(ammo_types));
	memset(ammo_max, 0, sizeof(int));
	ammo_types[0] = "none";
	num_ammo_types = 1;

	// zero clip types array
	memset(clip_types, 0, sizeof(clip_types));
	memset(clip_max, 0, sizeof(int));
	clip_types[0] = "none";
	num_clip_types = 1;
		
	memset(weapons, 0, sizeof(weapons));

	if (!already_allocated) {
		num_weapons = trap_fs_get_file_list(BG_WEAPONS_FOLDER, "", file_list,
											sizeof(file_list));

		if (!num_weapons)
			g_error("No weapon files found in %s", BG_WEAPONS_FOLDER);
		if (num_weapons > MAX_WEAPONS)
			g_error("Max number of weapons allowed is %d, found %d", MAX_WEAPONS,
					num_weapons);

		weapon = file_list;
		for (i = 0; i < num_weapons; i++) {
			len = strlen(weapon);

			g_dprintf("Getting weapon file \"%s/%s\" for parsing\n",
					  BG_WEAPONS_FOLDER, weapon);

			weapons[i] = weapon;
			weapon += len + 1;
		}

		qsort(weapons, num_weapons, sizeof(char *), compare_weapon_file_names);
		config_string[0] = '\0';

		for (i = 0; i < num_weapons; i++) {
			if (i)
				strcat(config_string, " ");

			strcat(config_string, weapons[i]);
		}
		trap_set_config_string(CS_WEAPONS, config_string);

		bg_parse_weapon_files(weapons, num_weapons);
	} else {
		bg_num_weapons = 0;

		for (i = 1; (i < MAX_WEAPONS && bg_weapons[i] != NULL); i++)
			bg_num_weapons++;
	}

	bg_set_ads_trans_times();
	bg_setup_ammo_indexes();
	bg_setup_shared_ammo_indexes();
	bg_setup_clip_indexes();
	bg_fill_in_weapon_items();
	bg_setup_alt_weapon_indexes();
	bg_setup_weapon_hint_strings();

	g_printf("total weapons: %d\n", bg_num_weapons);
}
