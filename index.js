const anitomy = require('./build/Release/anitomy-js.node')

const nativeParseAsync = anitomy.parseAsync
const nativeParseSync = anitomy.parseSync

const slice = (args) => args.slice(0, 2)
const parseAsync = (...args) => nativeParseAsync(...slice(args))
const parseSync = (...args) => nativeParseSync(...slice(args))
const parse = parseAsync

module.exports = { parse, parseAsync, parseSync }
