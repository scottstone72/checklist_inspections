/**
 * Created by Scott Johnstone on 28/08/14.
 */

(function ($) {
  Drupal.behaviors.addHighlight = {
    attach: function (context, settings) {

      // If radio button is clicked change the background of the fieldset
      $('.radios').click(function() {
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