import os
from subprocess import call




files = [
	'message/address.cpp'
	, 'message/alert.cpp'
	, 'message/alert_payload.cpp'
	, 'message/block.cpp'
	, 'message/block_transactions.cpp'
	, 'message/compact_block.cpp'
	, 'message/fee_filter.cpp'
	, 'message/filter_add.cpp'
	, 'message/filter_clear.cpp'
	, 'message/filter_load.cpp'
	, 'message/get_address.cpp'
	, 'message/get_block_transactions.cpp'
	, 'message/get_blocks.cpp'
	, 'message/get_data.cpp'
	, 'message/get_headers.cpp'
	, 'message/header.cpp'
	, 'message/headers.cpp'
	, 'message/heading.cpp'
	, 'message/inventory.cpp'
	, 'message/inventory_vector.cpp'
	, 'message/memory_pool.cpp'
	, 'message/merkle_block.cpp'
	, 'message/not_found.cpp'
	, 'message/ping.cpp'
	, 'message/pong.cpp'
	, 'message/prefilled_transaction.cpp'
	, 'message/reject.cpp'
	, 'message/send_compact.cpp'
	, 'message/send_headers.cpp'
	, 'message/transaction.cpp'
	, 'message/verack.cpp'
	, 'message/version.cpp'
	, 'message/xverack.cpp'
	, 'message/xversion.cpp'

	, 'wallet/bitcoin_uri.cpp'
	, 'wallet/ec_private.cpp'
	, 'wallet/ec_public.cpp'
	, 'wallet/encrypted_keys.cpp'
	, 'wallet/message.cpp'
	, 'wallet/payment_address.cpp'
]

def process():
    for f in files:
        # include/kth/domain/
        # src
        filename, file_extension = os.path.splitext(f)
        # print(filename)
        # print(file_extension)
        # print(filename.split('/')[-1])
        class_name = filename.split('/')[-1]

        inc = os.path.join('include/kth/domain/', filename) + '.hpp'
        src = os.path.join('src/', f)
        print(inc)
        print(src)


        # print('git commit -m "fixes in {} class"'.format(class_name))

        ret = call(["git", "add", inc])
        if ret != 0:
            print('error 1')
            print(inc)
            return

        print(call(["git", "add", src]))
        if ret != 0:
            print('error 2')
            print(src)
            return

        print(call(['git', 'commit', '-m', 'canonical C++ way for {} class'.format(class_name)]))
        if ret != 0:
            print('error 3')
            return


process()