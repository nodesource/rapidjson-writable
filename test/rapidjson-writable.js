'use strict'

const test = require('tape')
const exec = require('child_process').exec
const path = require('path')
const minify = path.join(__dirname, '..', 'bin', 'test_minify')

const largeObject = {
    rootKey: 'hello'
  , rootObject: {
      true: true
    , false: false
    , one: 1
    }
  , rootArray: [
      'hi', 1, 1.22, true
    ]
}

test('\nround tripping different small JSON payloads with chunkSize 64', function(t) {
  /* eslint-disable comma-spacing */
  const payloads = [
      [ 'emptyObject' , { } ]
    , [ 'emptyArray'  , [ ] ]
    , [ 'numberKey'   , { number: 1 } ]
    , [ 'objectKey'   , { object: { } } ]
    , [ 'nestedObject', { outer: { inner: { number: 1 } } } ]
    , [ 'largeObject' , largeObject ]
  ]
  /* eslint-enable comma-spacing */

  payloads.forEach(runTest)

  function runTest(payload) {
    const name = payload[0]
    const obj = payload[1]

    test('\n' + name, function(t) {
      const json = JSON.stringify(obj)
      const cmd = `echo '${json}' | ${minify} 64`
      console.log(cmd)
      exec(cmd, onresult)

      function onresult(error, stdout, stderr) {
        t.iferror(error, 'should not error')
        t.equal(stderr.toString(), 'OK', 'finishes and prints OK')
        try {
          const res = JSON.parse(stdout.toString())
          t.deepEqual(res, obj)
        } catch (e) {
          t.iferror(e)
        }
        t.end()
      }
    })
  }
  t.end()
})

test('\nround tripping package.json with different chunk sizes', function(t) {
  const pack = require('../package.json')

  const chunkSizes = [ 1E6, 1E4, 640000, 640, 64 ]
  chunkSizes.forEach(runTest)

  function runTest(chunkSize) {
    test('\nchunk size ' + chunkSize, function(t) {
      const args = [ chunkSize, require.resolve('../package.json') ]
      const cmd = `${minify} ${args.join(' ')}`
      console.log(cmd)
      exec(cmd, onresult)

      function onresult(error, stdout, stderr) {
        t.iferror(error, 'should not error')
        t.equal(stderr.toString(), 'OK', 'finishes and prints OK')
        try {
          const res = JSON.parse(stdout.toString())
          t.deepEqual(res, pack)
        } catch (e) {
          t.iferror(e)
        }
        t.end()
      }
    })
  }
  t.end()
})
