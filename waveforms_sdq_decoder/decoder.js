// Microsecond constant independent of the sampling rate.
const MICROSECOND = round(hzRate / 1000000);

// Minimum pulse width for zero words, approximately 4 us.
const ZERO_THRESHOLD = MICROSECOND * 4;

// Minimum pulse width for break words, approximately 8 us.
const BREAK_THRESHOLD = MICROSECOND * 8;

const FLAG_DATA = 1;
const FLAG_BREAK = 2;

var currentByteStart = -1;
var currentBitStart = 0;
var currentByte = [];

var previousSample = 1;
for (var i = 0; i < rgData.length; i++) {
  const currentSample = rgData[i];

  const atFallingEdge = previousSample == 1 && currentSample == 0;
  const atRisingEdge = previousSample == 0 && currentSample == 1;

  if (atFallingEdge) {
    // This is a falling edge. If this is the start of a byte, mark it and set
    // the decoding state accordingly. In either case, update the current bit
    // start index.
    if (currentByteStart < 0) currentByteStart = i;

    currentBitStart = i;
  }

  if (currentByteStart > 0 && atRisingEdge) {
    const currentPulseWidth = i - currentBitStart;

    // This is a rising edge and a byte is currently being decoded; need to
    // make sense of whatever bit/word is at hand...
    if (currentPulseWidth > BREAK_THRESHOLD) {
      // Pulse width exceeds the break threshold, mark this span as a break.
      for (var j = currentByteStart; j < i; j++) rgFlag[j] = FLAG_BREAK;

      // Reset the current byte start position since this pulse will not be
      // considered when forming a byte.
      currentByteStart = i;
    } else {
      // This is a valid word/bit, decode accordingly...
      currentByte.push(currentPulseWidth > ZERO_THRESHOLD ? 0 : 1);
    }
  }

  // Once eight bits/words have been collected, assume the protocol is behaving
  // normally and decode the byte.
  if (currentByte.length == 8) {
    var byteValue = 0;
    for (var bitIndex = 0; bitIndex < 8; bitIndex++)
      byteValue |= currentByte[bitIndex] << bitIndex;

    for (var j = currentByteStart; j < i; j++) {
      rgValue[j] = byteValue;
      rgFlag[j] = FLAG_DATA;
    }

    // Reset decoding state.
    currentByte = [];
    currentByteStart = -1;
  }

  previousSample = currentSample;
}
