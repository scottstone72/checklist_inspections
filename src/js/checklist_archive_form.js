
(function ($) {

  Drupal.behaviors.checklist_approval_form_collaspe = {
    attach: function (context, settings) {

      //$('input:checkbox').parent().not('.checkboxes').remove();

      $('.approval-fieldset', context).once(function (context) {
        $(this).hide(); // Hide the forms on page load.
      });

      // Toggle fieldsets when user clicks show Details or the Cancel button
      $(".show-details, .cancel-btn-val", context).click(function () {

        table_num = $(this).attr('value');
        selFieldset = 'fieldset#container-'+ table_num;

        var selectdiv = $(selFieldset);
        var elem = selectdiv.slideToggle(600);
        $('.approval-fieldset').not(elem).hide();

        if(table_num)  {
          addRepairStats(table_num);
        }
      }); // End click event

    }
  };

  function addRepairStats(tableNum) {

    var repairStats = Drupal.settings.checklist_approval_data;
    // Filter out the order that was selected.
    $.each(JSON.parse(repairStats), function (key, value) {

      $.each(value, function (keyVal, val) {

        if(key === tableNum) {
          // Add active class to all applicable id's.
          var id = '#' + keyVal;
          $(id).parent().addClass('active');
          $(id).prop('checked', true);

          if (Number(val) === 1) {
            $(id).parents('fieldset.ratings-container').addClass('satisfactory');
          }
          if (Number(val) === 2) {
            $(id).parents('fieldset.ratings-container').addClass('deficient');

          }
        }
      });
    }); // End outside each loop
  }


  Drupal.behaviors.checklist_modal_radios = {
    attach: function (context, settings) {

      // If radio button is clicked change the background of the fieldset
      $('.radios', context).click(function() {
        var radioVal = $(this).children('input').attr('value');

        if (radioVal === '1') {
          $(this).parents('.ratings-container').addClass('satisfactory');
          $(this).parents('.ratings-container').removeClass('deficient');
        }
        else if (radioVal === '2') {
          $(this).parents('.ratings-container').addClass('deficient');
          $(this).parents('.ratings-container').removeClass('satisfactory');
        }
        else {
          $(this).parents('.ratings-container').removeClass('satisfactory');
          $(this).parents('.ratings-container').removeClass('deficient');
        }
      }); // End the radio click function
    }
  };


})(jQuery);