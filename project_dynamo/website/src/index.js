import "./styles/styles.css";

//#region main
document.querySelector("#led-on-button").addEventListener("click", () => {
    console.log("Turning LED ON");
    fetch("/led/on");
});
document.querySelector("#led-off-button").addEventListener("click", () => {
    console.log("Turning LED OFF");
    fetch("/led/off");
});
//#endregion main