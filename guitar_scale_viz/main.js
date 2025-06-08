const NOTES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];

/**
 * XXX: Beware that the notes in each tuning are in "backwards" order.
 */
const TUNINGS = {
  6: {
    "E Standard": ["E", "B", "G", "D", "A", "E"],
    "Drop D": ["E", "B", "G", "D", "A", "D"],
    "Drop B": ["C#", "G#", "E", "B", "F#", "B"],
    "Drop A": ["B", "F#", "D", "A", "E", "A"],
  },
  7: {
    "B Standard": ["E", "B", "G", "D", "A", "E", "B"],
    "A Standard": ["D", "A", "F", "C", "G", "D", "A"],
    "Drop A": ["E", "B", "G", "D", "A", "E", "A"],
    "Drop G#": ["D#", "A#", "F#", "C#", "G#", "D#", "G#"],
    "Drop F#": ["C#", "G#", "E", "B", "F#", "C#", "F#"],
  },
};

const SCALES = {
  Major: [0, 2, 4, 5, 7, 9, 11],
  Minor: [0, 2, 3, 5, 7, 8, 10],
  "Harmonic Minor": [0, 2, 3, 5, 7, 8, 11],
  "Major Pentatonic": [0, 2, 4, 7, 9],
  "Minor Pentatonic": [0, 3, 5, 7, 10],
};

/**
 * Calculate the notes in a given scale.
 *
 * @param {string} root Root note of the scale
 * @param {number[]} intervals Array of semitone intervals for the scale
 * @returns {string[]} Array of note names in the scale
 */
function getScaleNotes(root, intervals) {
  return intervals.map(
    (interval) => NOTES[(NOTES.indexOf(root) + interval) % 12],
  );
}

const gStringCountSelect = document.getElementById("string-count");
const gTuningSelect = document.getElementById("tuning");
const gRootNoteSelect = document.getElementById("root-note");
const gScaleSelect = document.getElementById("scale");
const gFretCountInput = document.getElementById("fret-count-input");
const gShowAllNotesCheckbox = document.getElementById("show-all-notes-toggle");
const gFretboard = document.getElementById("fretboard");

/**
 * Create a new element, optionally with an initial class list and content.
 *
 * @param {string} type Type of element to create, e.g. 'div', 'span', 'p'
 * @param {string[]} [classes=[]] Optional list of classes to add to the element
 * @param {string} [content=''] Optional text/HTML content for the element
 * @returns {HTMLElement}
 */
function createElement(type, classes = [], content = "") {
  const element = document.createElement(type);

  if (classes.length > 0) {
    element.classList.add(...classes);
  }
  if (content) {
    element.innerHTML = content;
  }

  return element;
}

/**
 * Populate a select element with a list of options.
 *
 * @param {HTMLSelectElement} select Select element to populate
 * @param {string[]} values List of options/values
 */
function populateSelect(select, values) {
  select.innerHTML = "";

  values.forEach((value) => {
    const option = document.createElement("option");
    option.value = value;
    option.textContent = value;
    select.appendChild(option);
  });
}

/**
 * Update the tuning dropdown based on the selected number of strings.
 */
function updateTuningOptions() {
  const numStrings = gStringCountSelect.value;
  const tunings = Object.keys(TUNINGS[numStrings]).concat(["Custom"]);
  populateSelect(gTuningSelect, tunings);
}

/**
 * Get the current tuning as an array of note names.
 *
 * @returns {string[]} Array of notes, from string 1 (high E) to the lowest string.
 */
function getCurrentTuning() {
  const numStrings = parseInt(gStringCountSelect.value, 10);
  const tuningName = gTuningSelect.value;

  if (tuningName === "Custom") {
    const tuning = [];
    const selects = gFretboard.querySelectorAll(".open-string-header select");

    // If "custom" is selected but the dropdowns are not yet rendered, fall
    // back to the default.
    if (selects.length === 0) {
      return TUNINGS[numStrings][Object.keys(TUNINGS[numStrings])[0]];
    }

    selects.forEach((select) => tuning.push(select.value));
    return tuning;
  }

  return TUNINGS[numStrings][tuningName];
}

/**
 * Make the header cell for a string.
 *
 * @param {object} state Shared application state
 * @param {string} openNote Note of the open string
 * @returns {HTMLTableCellElement}
 */
function makeStringHeader(state, openNote) {
  const th = createElement("th", ["open-string-header"]);

  if (state.tuningName === "Custom") {
    const select = createElement("select");
    populateSelect(select, NOTES);
    select.value = openNote;
    select.addEventListener("change", render);
    th.appendChild(select);
    th.dataset.openNote = openNote; // Used for printing.
  } else {
    th.textContent = openNote;
  }

  return th;
}

/**
 * Make a fret cell for the fretboard table.
 *
 * @param {object} state Shared application state
 * @param {string} note Note for this fret
 * @param {boolean} isNut Whether this is the nut cell (fret 0)
 * @return {HTMLTableCellElement}
 */
function makeFretCell(state, note, isNut = false) {
  const fretCell = createElement("td", ["fret-cell"]);
  if (isNut) {
    fretCell.classList.add("nut-cell");
  }

  if (state.notesInScale.includes(note)) {
    const noteDisplay = createElement("div", ["note-display"], note);
    if (note === state.rootNote) {
      noteDisplay.classList.add("root");
    }
    fretCell.appendChild(noteDisplay);
  } else if (state.showAllNotes) {
    const fadedNote = createElement("div", ["faded-note"], note);
    fretCell.appendChild(fadedNote);
  }

  return fretCell;
}

/**
 * Make the main body of the fretboard table.
 *
 * @param {object} state Shared application state
 */
function makeFretboardBody(state) {
  const body = createElement("tbody");

  for (let i = 0; i < state.numStrings; i++) {
    const stringRow = createElement("tr");
    const openNote = state.currentTuning[i];
    const openNoteIndex = NOTES.indexOf(openNote);

    const stringHeader = makeStringHeader(state, openNote);
    stringRow.appendChild(stringHeader);

    for (let fret = 0; fret <= state.fretCount; fret++) {
      const noteIndex = (openNoteIndex + fret) % 12;
      const currentNote = NOTES[noteIndex];

      const fretCell = makeFretCell(state, currentNote);
      stringRow.appendChild(fretCell);
    }
    body.appendChild(stringRow);
  }

  return body;
}

/**
 * Make the numbered footer for the fretboard table.
 *
 * @param {object} state Shared application state
 */
function makeFretboardFooter(state) {
  const footer = createElement("tfoot");
  const row = createElement("tr");

  row.appendChild(createElement("th")); // Needed for alignment.

  for (let i = 0; i <= state.fretCount; i++) {
    const cell = createElement("th", [], i.toString());
    cell.scope = "col";
    row.appendChild(cell);
  }

  footer.appendChild(row);
  return footer;
}

const FRET_COUNT_DEFAULT = 17;

/**
 * Render all dynamic UI elements.
 */
function render() {
  const state = {
    numStrings: parseInt(gStringCountSelect.value, 10),
    tuningName: gTuningSelect.value,
    fretCount: parseInt(gFretCountInput.value, 10) || FRET_COUNT_DEFAULT,
    showAllNotes: gShowAllNotesCheckbox.checked,
    rootNote: gRootNoteSelect.value,
    currentTuning: getCurrentTuning(),
  };

  state.notesInScale = getScaleNotes(
    state.rootNote,
    SCALES[gScaleSelect.value],
  );

  gFretboard.innerHTML = "";

  const body = makeFretboardBody(state);
  const footer = makeFretboardFooter(state);

  gFretboard.append(body, footer);
}

function init() {
  gFretCountInput.value = FRET_COUNT_DEFAULT;

  populateSelect(gRootNoteSelect, NOTES);
  populateSelect(gScaleSelect, Object.keys(SCALES));
  updateTuningOptions();

  gStringCountSelect.addEventListener("change", () => {
    updateTuningOptions();
    gTuningSelect.selectedIndex = 0;

    render();
  });

  gTuningSelect.addEventListener("change", render);
  gRootNoteSelect.addEventListener("change", render);
  gScaleSelect.addEventListener("change", render);
  gFretCountInput.addEventListener("change", render);
  gShowAllNotesCheckbox.addEventListener("change", render);

  render();
}

document.addEventListener("DOMContentLoaded", init);
