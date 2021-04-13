function openAlert() {
    window.alert("Hello World");
}

function createPDF() {
    // Default export is a4 paper, portrait, using millimeters for units
    const doc = new jsPDF();

    doc.text("Hello world!", 10, 10);
    doc.save("test.pdf");
}