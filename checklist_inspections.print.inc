<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_category.class.inc.php');
drupal_add_css(drupal_get_path('module', 'checklist_inspections') . '/dist/css/min/check_inspect_print.min.css');
drupal_add_js(drupal_get_path('module', 'checklist_inspections') . '/dist/js/checklist_submit_print.min.js', array('scope' => 'footer'));

/**
 * Page callback for printing submitted Checklist Inspection.
 */
function checklist_inspections_callback_print() {

  $repair_data = $_SESSION['repair_data'];
  $repair_status = unserialize($repair_data['repair_data']);

  // unpack our session data
  $unit_id = $repair_data['unit_id'];
  $category = $repair_data['category'];
  $date_submitted = $repair_data['date_submitted'];
  $order_num = $repair_data['order_num'];
  $inspected_by = $repair_data['inspected_by'];
  $comments = $repair_data['comments'];
  $follow_up_alert = $repair_data['follow_up_alert'];

  // Send image through the theme_image function.
  $image_path = drupal_get_path('theme', 'ponderosa_bootstrap') . '/images/ponderosa_logo.png';
  $variables = array(
    'path' => $image_path,
    'alt' => 'Company Logo',
    'title' => 'Company Logo',
    'attributes' => array('class' => 'company-image', 'id' => 'company-image'),
  );
  $img = theme('image', $variables);

  global $user;
  $output = array();

  $output['container'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>' . $img . '</div>',
  );

  $output['container']['checklist_title'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="checklist-title"><h2>Checklist Inspections</h2></div>',
  );

  $output['container']['category'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="category"><h3><strong>'. $category .'</strong></h3></div>',
  );

    $output['container']['unit_id_select'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="unit_id"><h4><strong>'. $unit_id .'</strong></h4></div>',
    );

  $output['container']['order_num'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="order-num"><strong>Order #: &nbsp</strong>'. $order_num .'</div>',
  );

  $output['container']['date_submitted'] = array(
    '#type' => 'markup',
    '#markup' => '<div><strong>Date: &nbsp</strong>'. $date_submitted .'</div>',
        '#prefix' => '<div id="date-submitted">',
        '#suffix' => '</div>',
  );


  $output['container']['inspected_by'] = array(
    '#type' => 'markup',
    '#markup' => '<div><strong>Inspected By: &nbsp</strong>'. $inspected_by .'</div>',
        '#prefix' => '<div id="inspected-by">',
        '#suffix' => '</div>',
  );


  $output['container']['checklist_message'] = array(
    '#markup' => '<div id="user-instructions"><strong>S</strong> = <em>Satisfactory</em>&nbsp
        <strong>D</strong> = <em>Deficient</em>&nbsp <strong>N/A</strong> = <em> Not Applicable</em></div>'
  );

  // Fieldset to contain each row of information
  $output['container']['equip_state'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="edit-equip-state">State of Repair</div>',
//    '#id' => t('edit-equip-state'),
  );

    $category_obj = new ChecklistCategories();
    $category_list = $category_obj->build_category_list($category);
    $rows = array();
    $repair_states = array();

    foreach ($category_list as $box => $rad) {
      // Reorganize into the array format we need
      if ($box === 'title') {
        foreach ($rad as $y => $x) {
          $rows[$y] = $x;
        }
      }
      if ($box === 'repair_state') {
        foreach ($rad as $b => $c) {
          $repair_states[$b] = $c;
        }
      }
    }


    // Start building the all the checkboxes inside each fieldset.
    foreach ($rows as $key => $state) {
      $output['container'][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('ratings-container')),
        '#tree' => TRUE,
      );

      // Add the repair states for each row if it is a string value
      if(gettype($state) === 'string') {
        $output['container'][$key][$state] = array(
          '#type' => 'markup',
          '#markup' => t("<strong class='repair-title'>@state</strong>", array('@state' => $state)),
          '#attributes' => array(
            'id' => array($key),
            //          'class' => array('col-sm-4'),
          ),
        );
      }

      foreach ($repair_states as $key_val => $cond) {
        // Build our radio buttons
        $output['container'][$key]['radios'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="radio-buttons-right">
                          <div class="repair-radio-buttons"><span id="order-'. $order_num . '-' . $key . '_radio_1" value="1"><strong>S</strong></span></div>
                          <div class="repair-radio-buttons"><span id="order-'. $order_num . '-' . $key . '_radio_2" value="2"><strong>D</strong></span></div>
                          <div class="repair-radio-buttons"><span id="order-'. $order_num . '-' . $key . '_radio_3" value="3"><strong>N/A</strong></span></div>
                      </div>'
        );

        // Compare key value in $repairs_state array with key value in $rows titles
        // being cycled in the outside foreach loop.
        if ($key_val == $key) {

          $output['container'][$key]['checkbox_group'] = array(
            '#prefix' => '<div class="checkbox-group">',
            '#suffix' => '</div>',
          );

          // If there is a match than print checkboxes for each repair state value
          foreach ($cond as $val) {
            $output['container'][$key]['checkbox_group'][$val] = array(
              '#type' => 'markup',
              '#markup' => '<div  class="checkbox-values">
                              <span name="' . $key . '_' . $val . '" id="order-'. $order_num . '-' . $key . '_' . $val . '">' . $val . '</span></div>'
            );
          }
        }

        $is_true = FALSE;
        switch ($key) {
          case 'wiring':
            $is_true = TRUE;
            break;
          case 'dry_amps':
            $is_true = TRUE;
            break;
          case 'pumping_amps':
            $is_true = TRUE;
            break;
          case 'pressure':
            $is_true = TRUE;
            break;
          case 'propane_level':
            $is_true = TRUE;
            break;
          case 'certification':
            $is_true = TRUE;
            break;
          case 'tire_pressure_tires':
            $is_true = TRUE;
            break;
          case 'engine_hours':
            $is_true = TRUE;
            break;
          case 'oil_level':
            $is_true = TRUE;
            break;
          case 'fuel_level':
            $is_true = TRUE;
            break;
          case 'trans_and_trans_case':
            $is_true = TRUE;
            break;
          case 'differentials':
            $is_true = TRUE;
            break;
          case 'hydraulic_tank':
            $is_true = TRUE;
            break;
          case 'fuel_tank':
            $is_true = TRUE;
            break;
          case 'engine_coolant':
            $is_true = TRUE;
            break;
          case 'fire_extinguisher':
            $is_true = TRUE;
            break;
          case 'window_wipers_washer':
            $is_true = TRUE;
            break;
          case 'pvc':
            $is_true = TRUE;
            break;
          case 'consumables_couplers':
            $is_true = TRUE;
            break;
          case 'elbows':
            $is_true = TRUE;
            break;
          case 'duct_tape':
            $is_true = TRUE;
            break;
          case 'zip_ties':
            $is_true = TRUE;
            break;
          case 'slings_tarp_straps':
            $is_true = TRUE;
            break;
          case 'overall_condition_8dc':
            $is_true = TRUE;
            break;
          case 'threads_8dc':
            $is_true = TRUE;
            break;
          case 'overall_condition_5dc':
            $is_true = TRUE;
            break;
          case 'ambient_temp':
            $is_true = TRUE;
            break;
          case 'oil_level_gen':
            $is_true = TRUE;
            break;
          case 'heaters':
            $is_true = TRUE;
            break;
          case 'belts':
            $is_true = TRUE;
            break;
          case 'battery_charger':
            $is_true = TRUE;
            break;
          case 'battery_levels_cables':
            $is_true = TRUE;
            break;
          case 'leaks':
            $is_true = TRUE;
            break;
          case 'amp':
            $is_true = TRUE;
            break;
          case 'volts':
            $is_true = TRUE;
            break;
          case 'freq':
            $is_true = TRUE;
            break;
          case 'oil_pressure':
            $is_true = TRUE;
            break;
          case 'water_temp':
            $is_true = TRUE;
            break;
          case 'bc_amp':
            $is_true = TRUE;
            break;
          case 'rtm':
            $is_true = TRUE;
            break;
          case 'ats':
            $is_true = TRUE;
            break;
          case 'mechanic':
            $is_true = TRUE;
            break;
          default;
        }

        // Only print textfields that apply along with the value
        if ($is_true) {
          foreach ($repair_status as $x => $y) {
            foreach($y as $data_key => $data_val) {
            $data = 'order-'. $order_num . '-' . $key; // Need a match to print textbox value

            // Filter status till we get textbox value
              if($data === $data_key) {
                $output['container'][$key][$key . '_textbox'] = array(
                  '#type' => 'markup',
                  '#markup' => '<div class="status-comments">: <span class="stat-text">' . $data_val . '</span></div>'
                );
              }
             }
          }
        }
      }
    }


  $output['container']['comments'] = array(
    '#type' => 'markup',
    '#markup' => t('<div id="comments"><p style="display: inline-block">Comments: &nbsp @comments</p></div>', array('@comments' => $comments)),
  );


  $output['container']['follow_up_by'] = array(
    '#type' => 'markup',
    '#markup' => t('<div id="follow-up-alert">Follow Up By: &nbsp@follow-up-alert</div>', array('@follow-up-alert' => $follow_up_alert)),
  );

  $output['container']['checklist_return_link'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="link-container"><ul class="link-list"><li>' . l(t(' <- back to Checklist Inspection Forms'), 'checklist_inspections',
        array('attributes' => array('class' => array('transfer-form-link')))) . "</li>",
  );

  $output['container']['home_return_link'] = array(
    '#type' => 'markup',
    '#markup' => '<li>' . l(t('Home'), '', array('attributes' => array('class' => array('home-link')))) . "</li>",
  );

  $output['container']['print_link'] = array(
    '#type' => 'markup',
    '#markup' => '<li id="print-link"><div id="print-this-page">Print Page</div></li></ul></div>',
  );


  drupal_add_js(array('checklist_inspections' => $repair_status), 'setting');

  return $output;
}