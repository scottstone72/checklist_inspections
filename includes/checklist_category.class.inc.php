<?php

class ChecklistCategories {

  function build_category_list($category = NULL) {

    if(($category === '2" Trash Pumps') || ($category === '4" Trash Pumps') || ($category ==='Flygt Pump')) {
      $category = '2" Trash Pumps';
    }

    if(($category === '8x40 Rig Mats') || ($category === '8x20 Rig Mats')) {
      $category = 'Rig Matting';
    }

    switch ($category) {
      // Build the row of checkboxes
      case '2" Trash Pumps':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'stand_base' => t('Stand/Base '),
            'volute' => t('Volute '),
            'handle' => t('Handle '),
            'impeller' => t('Impeller '),
            'wear_ring' => t('Wear Ring '),
            'oil' => t('Oil '),
            'seal' => t('Seal '),
            'motor_housing' => t('Motor Housing '),
            'junction_box' => t('Junction Box '),
            'rotor_stator' => t('Rotor/Stator '),
            'cable' => t('Cable '),
            'plug_ends' => t('Plug Ends '),
            'wiring' => t('Wiring '),
            'paint' => t('Paint '),
            'sn' => t('Serial # '),
            'dry_amps' => t('Dry Amps '),
            'pumping_amps' => t('Pumping Amps '),
            'pressure' => t('Pressure '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'stand_base' => array('Cracks', 'Damage', 'Loose'),
            'volute' => array('Damage', 'Cracks', 'Camlock'),
            'handle' => array('Damage', 'Loose', 'Bolts'),
            'impeller' => array('Damage', 'Wear', 'Loose'),
            'wear_ring' => array('Damage', 'Wear'),
            'oil' => array('FluidLevels', 'Cleanliness'),
            'seal' => array('Leaks', 'WaterInOil'),
            'motor_housing' => array('Damage', 'Cracks', 'Seals'),
            'junction_box' => array('Damage', 'Leaks'),
            'rotor_stator' => array('Damage', 'HeatSigns'),
            'cable' => array('Damage', 'Wiring'),
            'plug_ends' => array('Damage', 'Wear', 'Arcing'),
            'wiring' => array('208V', '480V'),
            'paint' => array('Damage', 'Condition'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;
      case 'Shale Bins':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'floor' => t('Floor '),
            'fluid_brem' => t('Fluid Brem '),
            'traction_bars' => t('Traction Bars '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'skid' => array('Wear', 'Damage', 'Cracks'),
            'floor' => array('Damage', 'Dents', 'Cracks'),
            'fluid_brem' => array('Damage', 'Cracks', 'Leaks'),
            'traction_bars' => array('Wear', 'Damage'),
            'paint' => array('Damaged', 'Clean'),
            'decals' => array('Installed', 'Damaged'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;

      case 'Floc Tanks':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'walk_away' => t('Walk Way '),
            'boom_arm' => t('Boom Arm '),
            'winch' => t('Winch '),
            'cable' => t('Cable '),
            'air_bags' => t('Air Bags '),
            'camlocks' => t('Camlocks '),
            'valves' => t('Valves '),
            'suck_out_lines' => t('Suck Out Lines '),
            'gates' => t('Gates '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
          ),
          'repair_state' => array(
            'cleaniless' => array('Water', 'Mud', 'SlipTankInside'),
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'skid' => array('Wear', 'Damage', 'Cracks'),
            'walk_away' => array('Function', 'Damage', 'Cracks'),
            'boom_arm' => array('Function', 'Damage', 'Cracks'),
            'winch' => array('Damage', 'Wear'),
            'cable' => array('Damage', 'Wear'),
            'air_bags' => array('Leaks', 'Wear', 'Damage'),
            'camlocks' => array('Damage', 'Wear', 'Gaskets'),
            'valves' => array('Condition', 'Leaks'),
            'suck_out_lines' => array('2"OffBottom', 'Secure'),
            'gates' => array('Damage', 'Function'),
            'paint' => array('Damage', 'Clean'),
            'decals' => array('Installed', 'Damaged'),
            'sn' => array('Installed', 'Visible'),
          ),
      );
        break;

      case 'Flare Tanks':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'hammer_unions' => t('Hammer Unions '),
            'ignitors' => t('Ignitors '),
            'ignitor_box' => t('Ignitor Box '),
            'electrical' => t('Electrical '),
            'propane_lines' => t('Propane Lines '),
            'winch_cable' => t('Winch & Cable '),
            'hinges_doors' => t('Hinges / Doors '),
            'bottle_cart' => t('Bottle Cart '),
            'propane_level' => t('Propane Level '),
            '3x8_6x8' => t('3x8 / 6x8 '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
        ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'skid' => array('Wear', 'Damage', 'Cracks'),
            'hammer_unions' => array('Function', 'Wear', 'Damage', 'Rating', 'Size'),
            'ignitors' => array('Function', 'Damage'),
            'ignitor_box' => array('Function', 'Damage', 'Wiring'),
            'electrical' => array('Function', 'Damage', 'Wiring'),
            'propane_lines' => array('Damage', 'Cracks', 'Leaks'),
            'winch_cable' => array('Function', 'Wear', 'Damage'),
            'hinges_doors' => array('Function', 'Damaged', 'Greased'),
            'bottle_cart' => array('Function', 'Wear', 'Damage'),
            '3x8_6x8' => array('Function', 'Wear', 'Damage'),
            'paint' => array('Damage', 'Cleanliness'),
            'decals' => array('Damage', 'Installed'),
            'sn' => array('Installed', 'Visible'),
          ),
      );
        break;

      case 'Bop Spools':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'flanges' => t('Flanges '),
            'plugs_valves' => t('Plugs / Valves '),
            'gaskets' => t('Gaskets '),
            'studs_nuts' => t('Studs / Nuts '),
            'paint' => t('Paint '),
            'sn' => t('SN #  '),
            'certification' => t('Certification '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'flanges' => array('Damage', 'Cracks', 'Welds'),
            'plugs_valves' => array('Damage', 'Loose', 'Function'),
            'gaskets' => array('Damage', 'Wear', 'Size'),
            'studs_nuts' => array('Damage', 'Wear'),
            'paint' => array('Damage', 'Condition'),
            'sn' => array('Stamped', 'Visible'),
            'certification' => array('InService', 'Expiration3yr'),
          ),
        );
        break;

      case 'Bail Extensions':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'connectors' => t('Connectors '),
            'safety_pins' => t('Safety Pins '),
            'paint' => t('Paint '),
            'sn' => t('SN # '),
            'certification' => t('Certification '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'connectors' => array('Damage', 'Wear', 'Cracks'),
            'safety_pins' => array('Damage', 'Wear', 'Function'),
            'paint' => array('Damage', 'Condition'),
            'sn' => array('Stamped', 'Visible'),
            'certification' => array('InService', 'Expiration3yr'),
          ),
        );
        break;

      case '400BBL Tanks':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'winch_sling' => t('Winch Sling '),
            'float_indicator' => t('Float / Indicator '),
            'butterfly_valve' => t('Butterfly Valve '),
            'hammer_unions' => t('Hammers Unions '),
            'fall_arrest' => t('Fall Arrest '),
            'ladder' => t('Ladder '),
            'vent' => t('Vent '),
            'hatch_seal' => t('Hatch Seal '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'skid' => array('Wear', 'Damage', 'Cracks'),
            'winch_sling' => array('Wear', 'Damage', 'Cracks'),
            'float_indicator' => array('Function', 'Damage', 'Cracks'),
            'butterfly_valve' => array('Leaks', 'Wear', 'Damage'),
            'hammer_unions' => array('Damage', 'Wear'),
            'fall_arrest' => array('Condition', 'Certification'),
            'ladder' => array('Damage', 'CrackedWelds'),
            'vent' => array('Clean', 'Functioning'),
            'hatch_seal' => array('Leaks', 'Damaged'),
            'paint' => array('Damaged', 'Clean'),
            'decals' => array('Installed', 'Damaged'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;
      case 'Pipe Racks':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'pins_chains' => t('Pins / Chains '),
            'paint' => t('Paint '),
            'sn' => t('SN # '),
          ),
          'repair_state' =>array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'skid' => array('Wear', 'Damage', 'Cracks'),
            'pins_chains' => array('Function', 'Damage', 'Wear'),
            'paint' => array('Damage', 'Clean'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;

      case 'Loaders':
        $cat_list = array(
          'title' => array(
            'tires_chains_lug_stem' => t('Tires / Tire Chains / Lug Nuts / Stem Caps '),
            'bucket_forks_grapple' => t('Buckets / Forks / Grapple '),
            'lift_tilt_cylinders' => t('Lift / Tilt Cylinders '),
            'lines_hoses' => t('Lines / Hoses '),
            'frame_arms' => t('Frame / Arms '),
            'underneath_machine' => t('Underneath Machine '),
            'engine_hours' => t('Engine Hours '),
            'trans_and_trans_case' => t('Transmission / Transmission Case '),
            'differentials' => t('Differentials '),
            'hydraulic_tank' => t('Hydraulic Tank '),
            'fuel_tank' => t('Fuel Tank '),
            'battery_compartment' => t('Battery Compartment '),
            'exterior_lights' => t('Exterior Lights '),
            'engine_oil' => t('Engine Oil '),
            'engine_coolant' => t('Engine Coolant '),
            'radiator' => t('Radiator '),
            'all_hoses' => t('All Hoses '),
            'fuel_filters_water_sep' => t('Fuel Filters / Water Separator '),
            'all_belts' => t('All Belts '),
            'air_filter' => t('Air Filter '),
            'overall_engine_comp' => t('Overall Engine Compartment '),
            'handhold_steps' => t('Handholds / Steps '),
            'rops' => t('ROPS >'),
            'fire_extinguisher' => t('Fire Extinguisher '),
            'windows' => t('Windows >'),
            'window_wipers_washer' => t('Windows Wipers / Washer '),
            'doors' => t('Doors >'),
            'seat' => t('Seat >'),
            'seat_belt' => t('Seat Belt >'),
            'horn_backup_alarm' => t('Horn / Backup Alarm '),
            'mirrors' => t('Mirrors >'),
            'gauges_indicators' => t('Gauges / Indicators '),
            'switches' => t('Switches '),
            'controls' => t('Controls '),
            'overall_interior' => t('Overall Interior '),
          ),
          'repair_state' => array(
            'tires_chains_lug_stem' => array('Inflation', 'Damage', 'Wear'),
            'bucket_forks_grapple' => array('Damage', 'Wear', 'Function'),
            'lift_tilt_cylinders' => array('Damage', 'Wear', 'Leaks'),
            'lines_hoses' => array('Damage', 'Wear', 'Leaks'),
            'frame_arms' => array('Damage', 'Wear'),
            'underneath_machine' => array('Damage', 'Leaks'),
            'engine_hours' => array('TotalHours'),
            'trans_and_trans_case' => array('Leaks', 'FluidLevel'),
            'differentials' => array('Leaks', 'FluidLevel'),
            'hydraulic_tank' => array('Leaks', 'FluidLevel'),
            'fuel_tank' => array('Leaks', 'FluidLevel'),
            'battery_compartment' => array('Cleanliness', 'LooseNuts'),
            'exterior_lights' => array('Function', 'Damage', 'Wiring'),
            'engine_oil' => array('FluidLevel'),
            'engine_coolant' => array('FluidLevel'),
            'radiator' => array('Debris', 'Damage', 'Leaks'),
            'all_hoses' => array('Cracks', 'Wear', 'Leaks'),
            'fuel_filters_water_sep' => array('Leaks', 'DrainIfEquipped'),
            'all_belts' => array('Tension', 'Wear', 'Cracks'),
            'air_filter' => array('Restriction Indication'),
            'overall_engine_comp' => array('Trash', 'Dirt', 'Leaks'),
            'handhold_steps' => array('Condition'),
            'rops' => array('Damage', 'Loose'),
            'fire_extinguisher' => array('Charged', 'Damaged', 'AnnualInspection'),
            'windows' => array('Broken', 'Cleanliness'),
            'window_wipers_washer' => array('Wear', 'Damage', 'FluidLevel'),
            'doors' => array('Function'),
            'seat' => array('Condition'),
            'seat_belt' => array('Adjustment', 'Wear'),
            'horn_backup_alarm' => array('ProperFunction'),
            'mirrors' => array('Damage', 'Adjustment'),
            'gauges_indicators' => array('Damage', 'Function'),
            'switches' => array('Damage', 'Function'),
            'controls' => array('Damage', 'Function'),
            'overall_interior' => array('Cleanliness', 'LooseNuts'),
          ),
        );
        break;

      case 'Rig Matting':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'runners' => t('Runners '),
            'wood' => t('Wood '),
            'sn' => t('SN # '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks'),
            'runners' => array('Wear', 'Damage', 'Cracks'),
            'wood' => array('Damage', 'Rot', 'Wear'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;

      case 'Light Towers':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness >  OverAll Condition'),
            'damages' => t('Damages '),
            'hitch_frame' => t('Hitch & Frame '),
            'safety_chains' => t('Safety Chains '),
            'outriggers' => t('Outriggers '),
            'wheel_bearings' => t('Wheel Bearings '),
            'tire_pressure_tires' => t('Tire Pressure / Tires '),
            'trailer_lights_plugs' => t('Trailer Lights / Plugs '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
            'hinges_doors' => t('Hinges / Doors '),
            'winch_cable' => t('Winch / Cable '),
            'mast_lightbar' => t('Mast / Light Bar '),
            'flood_lights' => t('Flood Lights '),
            'light_jbox' => t('Light J Box '),
            'battery_connections' => t('Batteries & Connections '),
            'wiring_condition' => t('Wiring Condition '),
            'engine_hours' => t('Engine Hours '),
            'engine_comp_hoses' => t('Engine Compressor Hoses '),
            'oil_level' => t('Oil Level '),
            'coolant_radiator' => t('Coolant & Radiator '),
            'gauges' => t('Gauges '),
            'switches' => t('Switches >'),
            'fuel_filters' => t('Fuel Filters '),
            'water_sep' => t('Water Separator '),
            'fuel_level' => t('Fuel Level '),
            'air_level' => t('Air Levels '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'hitch_frame' => array('Wear', 'Damage', 'Loose'),
            'safety_chains' => array('Wear', 'Damage', 'Loose'),
            'outriggers' => array('Function', 'Wear', 'Damage'),
            'wheel_bearings' => array('Greased', 'Loose'),
            'tire_pressure_tires' => array('Inflation', 'Wear', 'Damage'),
            'trailer_lights_plugs' => array('Function', 'Wear', 'Damage'),
            'paint' => array('Overall Condition'),
            'decals' => array('Overall Condition'),
            'sn' => array('Installed', 'Stamped'),
            'hinges_doors' => array('Function', 'Damage', 'Wiring'),
            'winch_cable' => array('Function', 'Wear', 'Damage'),
            'mast_lightbar' => array('Function', 'Wear', 'Damage'),
            'flood_lights' => array('Function', 'Clean', 'Damage'),
            'light_jbox' => array('Clean', 'Dry', 'Sealed'),
            'battery_connections' => array('Secure', 'Clean', 'Charged'),
            'wiring_condition' => array('Damaged', 'Secure', 'Function'),
            'engine_hours' => array('TotalHours'),
            'engine_comp_hoses' => array('Leaks', 'Wear', 'Damaged'),
            'oil_level' => array('Fluid Level'),
            'coolant_radiator' => array('FluidLevel', 'Debris', 'Leaks'),
            'gauges' => array('Function', 'Damage'),
            'switches' => array('Function', 'Damage'),
            'fuel_filters' => array('Leaks', 'DrainIfEquipped'),
            'water_sep' => array('Leaks', 'DrainIfEquipped'),
            'fuel_level' => array('FluidLevel', 'Debris', 'Leaks'),
            'air_level' => array('Cleanliness', 'Sealed'),
          ),
        );
        break;

      case 'Pre-Mix Tanks':
        $cat_list = array(
          'title' => array(
            'cleanliness' => t('Cleanliness  >  OverAll Condition'),
            'damages' => t('Damages '),
            'welds' => t('Welds '),
            'skid' => t('Skid '),
            'hammer_unions' => t('Hammer Unions '),
            'aggitators_motors' => t('Aggitators / Motors '),
            'gear_box' => t('Gear Box '),
            'mixing_motor_pump' => t('Mixing Motor / Pump '),
            'mixing_hopper' => t('Mixing Hopper '),
            'electrical' => t('Electrical '),
            'cam_locks' => t('Camlocks '),
            'valves' => t('Valves '),
            'hosing' => t('Hosing '),
            'hinges_doors' => t('Hinges / Doors '),
            'lights' => t('Lights '),
            'safety_supplies' => t('Safety Supplies '),
            'paint' => t('Paint '),
            'decals' => t('Decals '),
            'sn' => t('SN # '),
          ),
          'repair_state' => array(
            'damages' => array('Damage', 'Wear'),
            'welds' => array('Damage', 'Cracks', 'Leaks'),
            'skid' => array('Damage', 'Wear', 'Cracks'),
            'hammer_unions' => array('Function', 'Wear', 'Damage'),
            'aggitators_motors' => array('Function', 'Damage'),
            'gear_box' => array('Oil Level', 'Leaks', 'Wear'),
            'mixing_motor_pump' => array('Rotation', 'Belts', 'Seals'),
            'mixing_hopper' => array('Damage', 'Wear', 'Drain'),
            'electrical' => array('Function', 'Damage', 'Wiring'),
            'cam_locks' => array('Handles', 'Gaskets'),
            'valves' => array('Leaks', 'Handles'),
            'hosing' => array('Inventoried'),
            'hinges_doors' => array('Function', 'Damage', 'Greased'),
            'lights' => array('Function', 'Damage'),
            'safety_supplies' => array('Installed', 'Inventoried'),
            'paint' => array('Damage', 'Cleanliness'),
            'decals' => array('Damage', 'Installed'),
            'sn' => array('Installed', 'Visible'),
          ),
        );
        break;

//      case 'Elevators':
//        $cat_list = array(
//          'title' => array(
//            'cleanliness' => t('Cleanliness  >  OverAll Condition'),
//            'general_condition' => t('General Condition '),
//            'pins_handles' => t('Pins / Handles '),
//            'rollpin_safetypins' => t('Roll Pin / Safety Pins '),
//            'latch_spring' => t('Latch / Spring '),
//            'shoulder' => t('Shoulder '),
//            'lubrication' => t('Lubrication '),
//            'function_check' => t('Function Check '),
//          ),
//          'repair_state' => array(
//            'damages' => array('Damage', 'Wear'),
//            'general_condition' => array('Damage', 'Cracks', 'Wear'),
//            'pins_handles' => array('Function', 'Damage', 'Wear'),
//            'rollpin_safetypins' => array('Installed'),
//            'latch_spring' => array('Proper Function'),
//            'shoulder' => array('Excessive Wear'),
//            'lubrication' => array('Properly Lubricated'),
//            'function_check' => array('Open / Close Flawlessly'),
//          ),
//        );
//        break;

      default:
        $cat_list = 'No Checklist Inspection Form has been created for this equipment category.';
               break;
    }
    return $cat_list;
  }
}