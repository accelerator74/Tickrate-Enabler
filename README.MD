## Tickrate Enabler by ProdigySim

### Description

Enables the "-tickrate" switch to be used from the command line to set the game's tickrate.

Patches Boomer Vomit behavior to fix an issue where vomit range scaled inversely with tickrate.

Removes global upper-limits on the max client data rate (was 30000),
and removes the (30k) limits on rate control cvars:
- sv_maxrate
- sv_minrate
- net_splitpacket_maxrate

### Instructions

1. Place tickrate_enabler.dll (Windows) or tickrate_enabler.so (Linux) in your server's addons folder.
2. Place tickrate_enabler.vdf (L4D2) or tickrate_enabler_l4d1.vdf (L4D1) in your server's addons folder.
3. Add "-tickrate <desired_tickrate>" to your server's launch parameters. e.g. -tickrate 100
4. Make sure the following convar settings are properly set in server.cfg or otherwise:

    sv_maxupdaterate 100
    sv_maxcmdrate 100
    fps_max 0
	sv_maxrate 0 // 0 = unlimited, but you can also set this manually. Default is 30k for 30tick, use math.
	// Setting higher sv_minrate can be used to force users to use higher data rates.
	
	Note that fps_max and sv_maxrate are hidden variables on most scenarios.

5. Done. Enjoy 100 tick L4D/L4D2 gameplay.

### Changelog / TODO

1.5: Contributed by Accelerator  
- Fixed CVomit::UpdateAbility offsets.  
- Some code optimization  

1.4: Contributed by $atanic $pirit  
- Fixed L4D2 CVomit::UpdateAbility Windows offsets.
- Fixed l4d Linux offsets.(Thanks to epzminion.) 
	
1.3:
- Fixed L4D1 CVomit::UpdateAbility linux offsets.  

1.2: thanks to vintik  
- Fixed maxrate patching on Windows (L4D1+2)  
- Added an extra maxrate patch on Linux that probably does nothing.  

1.1:
- Patches 2 hard coded max rate limits   
- Removes CVar Max on sv_max/minrate, net_splitpacket_maxrate  
- Some code reorganization  

1.0:
- Patches boomer vomit to behave properly on modified high tickrates  
- Code cleaner perhaps  

0.1: 
- -tickrate only pseudo-release  

TODO:
- Investigate other bugs  
- Remove 30K rate limit from HLTV clients.  

### Credits / License

tickrate_enabler is copyright Michael "ProdigySim" Busby 2012

Parts of this project contain GPLv3 code adapted from SourceMod (Allied Modders L.L.C.) and Left4Downtown2 (Igor Smirnov et. al.).

This entire project is released under the AlliedModders modified GPLv3.
