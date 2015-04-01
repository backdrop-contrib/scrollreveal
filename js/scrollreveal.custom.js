/**
* @file
* Javascript, modifications of DOM.
*
* Manipulates links to include scrollreveal data
*/

(function ($) {
  Backdrop.behaviors.scrollreveal = {
    attach: function (context, settings) {
      var triggers = Backdrop.settings.scrollreveal.triggers_fieldset;
      $.each(triggers, function(key, trigger) {
        var class_string = '';
        if (trigger.enter != "0") {
          class_string += 'enter ' + trigger.enter + ' ';
        };
        if (trigger.move != "0") {
          class_string += 'move ' + trigger.move + 'px ';
        };
        if (trigger.over != "0") {
          class_string += 'over ' + trigger.over + 's ';
        };
        if (trigger.after != "0") {
          class_string += 'after ' + trigger.after + 's ';
        }
        else if (trigger.wait != "0") {
          class_string += 'wait ' + trigger.wait + 's ';
        };
        $(trigger.element).attr( "data-sr", class_string.trim() );
      });
      var config = {
        after: Backdrop.settings.scrollreveal.config.after + 's',
        enter: Backdrop.settings.scrollreveal.config.enter,
        move: Backdrop.settings.scrollreveal.config.move + 'px',
        over: Backdrop.settings.scrollreveal.config.over + 's',
        easing: Backdrop.settings.scrollreveal.config.easing,
        viewportFactor: parseInt(Backdrop.settings.scrollreveal.config.viewportFactor),
        reset: (Backdrop.settings.scrollreveal.config.reset === "true"),
      };
      window.scrollReveal = new scrollReveal( config );
    }
  }

}(jQuery));
