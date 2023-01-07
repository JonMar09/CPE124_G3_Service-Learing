import TemperatureTracker from "./TemperatureTracker.js";

const app = document.getElementById("app");

const wt = new TemperatureTracker(app);

window.wt = wt;