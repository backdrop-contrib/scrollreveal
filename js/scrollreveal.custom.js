/**
 * @file
 * Activate scrollreveal on selected elements.
 */

(function($) {
  Backdrop.behaviors.scrollreveal = {
    attach: function(context, settings) {

      window.sr = ScrollReveal();
      var triggers = Backdrop.settings.scrollreveal;

      $.each(triggers, function(key, trigger) {
        var interval = trigger.interval ? parseInt(trigger.interval) : null;
        var config = {};

        config.origin = trigger.origin;
        if (trigger.distance) {
          config.distance = trigger.distance + 'px';
        };
        if (trigger.duration) {
          config.duration = parseInt(trigger.duration);
        };
        if (trigger.delay) {
          config.delay = parseInt(trigger.delay);
        };
        if (trigger.rotate_x || trigger.rotate_y || trigger.rotate_z) {
          config.rotate = {
            x: trigger.rotate_x ? parseInt(trigger.rotate_x) : 0,
            y: trigger.rotate_y ? parseInt(trigger.rotate_y) : 0,
            z: trigger.rotate_z ? parseInt(trigger.rotate_z) : 0
          };
        };
        if (trigger.opacity) {
          config.opacity = parseFloat(trigger.opacity);
        };
        if (trigger.scale) {
          config.scale = parseFloat(trigger.scale);
        };
        config.easing = trigger.easing;
        config.mobile = Boolean(trigger.mobile);
        config.reset = Boolean(trigger.reset);
        config.useDelay = trigger.useDelay;
        if (trigger.viewFactor) {
          config.viewFactor = parseFloat(trigger.viewFactor);
        };

        sr.reveal(trigger.element, config, interval);
      });

    }
  };
}(jQuery));

